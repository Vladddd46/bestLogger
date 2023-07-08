#include "Logger.hpp"

// Initialize static variables of Logger class.
std::map<std::thread::id, std::filesystem::path> Logger::logFiles;
std::map<std::filesystem::path, std::ofstream> Logger::fds;
std::mutex Logger::mutex_;
std::mutex Logger::logFilemutex_;
std::condition_variable Logger::conditionVariable_;
std::queue<Log> Logger::logStorage;
bool Logger::isInited =  false;


bool Logger::AddLogFile(const std::filesystem::path &filePath, const std::thread::id &threadId)
{
	std::lock_guard<std::mutex> lock(mutex_);
	bool result = true;

	std::string fileName = filePath.filename().string();
	std::string newFileName = utils::getCurrentDate() + "_" + fileName;
	std::filesystem::path newFilePath = filePath.parent_path() / newFileName;

	// logfile doest not exists.
	if (std::filesystem::exists(newFilePath) == false) 
	{
		result = utils::createFile(newFilePath); // also creates parent path if it does not exist.
	}

	if (std::filesystem::is_regular_file(newFilePath) == false)
	{
		std::cerr << "File can not be created: there is already directory by this path" << std::endl;
		result = false;
	}
	
	if (result == true) 
	{
		logFiles.insert(std::make_pair(threadId, newFilePath));

		// open logfiles.
		fds[newFilePath] = std::ofstream(newFilePath, std::ios::app);
	}
	return result;
}

bool Logger::ChangeLogFile(const std::filesystem::path &filePath, const std::thread::id &threadId)
{
	bool result = AddLogFile(filePath, threadId);
	return result;
}

void Logger::ShowLogFiles()
{
    for (const auto& pair : logFiles) 
    {
        std::cout << "ThreadId: " << pair.first << ", LogFile: " << pair.second << std::endl;
    }
}

void Logger::Deinit()
{
	// close all open log files
	for (auto& pair : fds) {
        pair.second.close();
    }
}

bool Logger::Init(const std::filesystem::path &filePath, const std::thread::id &threadId)
{
	int result = true;
	if (isInited == false)
	{
		result = Logger::AddLogFile(filePath, threadId);
		
		if (result != false)
		{
	    	std::thread processThread(&Logger::ProcessMessages);
	    	processThread.detach();

	    	std::thread reopenLogFilesThread(&Logger::ReopenLogFiles);
	    	reopenLogFilesThread.detach();
	    }
	    isInited = true;
	}
    return result;
}

void Logger::log(const std::string& logMessage)
{
	std::lock_guard<std::mutex> lock(mutex_);

	// creating Log object and adding it to queue
	Log tmpLog;
	tmpLog.msg = logMessage;
	tmpLog.threadId = std::this_thread::get_id();
	logStorage.push(tmpLog);

	// notifies ProcessMessages thread, that Log was added to logStorage.
	conditionVariable_.notify_one();
}

bool Logger::WriteLogMessageInFile(const std::string &logmsg, const std::thread::id &threadId)
{
	std::lock_guard<std::mutex> lock(logFilemutex_);
	int result = true;

	if (logFiles.find(threadId) == logFiles.end())
	{
		std::cerr << "Error: Can not write log: There is no logfile associated with thread: " << threadId << std::endl; 
		result = false;
	}
	else
	{
		std::filesystem::path filePath = logFiles[threadId];

	    if (fds[filePath].is_open())
	    {
	    	fds[filePath] << logmsg << std::endl;
	    }
	    else 
	    {
	    	std::cerr << "Error: file is not opened: " << filePath << std::endl;
	    	result = false;
	    }
	}
	return result;
}

void Logger::ProcessMessages() 
{
	while (true) 
	{
		std::unique_lock<std::mutex> lock(mutex_);
		conditionVariable_.wait(lock, []() { return !logStorage.empty(); });
		Log logObj = logStorage.front();
		logStorage.pop();

		// create log message.
		std::string datetime = utils::getCurrentDateTime();
		std::stringstream ss;
    	ss << logObj.threadId;
    	std::string threadIdStr = ss.str();

		std::string logmsg = datetime + ", " + threadIdStr + ", " + logObj.msg;

		WriteLogMessageInFile(logmsg, logObj.threadId);

		lock.unlock();
	}
}

void Logger::ReopenLogFiles() 
{
	while (true) {
        logFilemutex_.lock();
        for (const auto& pair : fds) 
        {
        	if (fds[pair.first].is_open())
        	{
	        	fds[pair.first].close();
	        	fds[pair.first] = std::ofstream(pair.first, std::ios::app);
	        }
	        else
	        {
	        	std::cerr << "Error: file is not opened: " << pair.first << std::endl;
	        }
        }
        logFilemutex_.unlock();
        // Sleep for 24 hours
        std::this_thread::sleep_for(std::chrono::hours(24));
    }
}
