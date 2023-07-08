#include "utils.hpp"

namespace utils
{

	bool createFile(const std::filesystem::path &filePath)
	{
		bool res = false;
		// logfile and parent path not exist
		if (std::filesystem::exists(filePath.parent_path()) == false && filePath.parent_path() != "")
		{
			std::filesystem::create_directories(filePath.parent_path());
		}
	    std::ofstream file(filePath);
	    if (file.is_open()) 
	    {
	        file.close();
	        res = true;
	    } 
	    else 
	    {
	        res = false;
	    }
	    return res;
	}

	std::string getCurrentDateTime()
	{
	    // Get the current system time
	    auto now = std::chrono::system_clock::now();
	    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	    // Convert the time to a struct tm
	    std::tm* timeinfo = std::localtime(&currentTime);

	    // Format the date and time
	    std::stringstream ss;
	    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
	    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	    ss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

	    // Convert the stringstream to a string
	    std::string datetime = ss.str();
	    return datetime;
	}

	std::string getCurrentDate()
	{
	    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	    std::tm* localTime = std::localtime(&currentTime);
	    std::stringstream ss;
	    ss << std::put_time(localTime, "%Y-%m-%d");
	    std::string formattedDate = ss.str();
	    return formattedDate;
	}

} // end of utils