#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <filesystem>
#include <map>
#include <fstream>
#include "utils.hpp"
#include <sstream>

#define DEFAULT_LOGFILE "logfile.txt"

/* @brief: representation of Log object.
 * @param:
 *        msg - log message.
 *        threadId - id of thread, which initiated writing this log.
 */
typedef struct
{
    std::string msg;
    std::thread::id threadId;
} Log_t;


class Logger
{
public:
    Logger() = delete;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    /* @brief: Initialize Logger.
     * @param:
     *        filePath - path to file, where logs will be written. By default - DEFAULT_LOGFILE
     *        threadId - id of thread, which logs will be written in filePath.
     *                   By default - thread, which called this method.
     */
    static bool Init(const std::filesystem::path &filePath=DEFAULT_LOGFILE, const std::thread::id &threadId=std::this_thread::get_id());

    /* @brief: deinitialize logger
     */
    static void Deinit();
    
    /* @brief: adds logfile for writing log associated with threadId.
     * @param:
     *        filePath - path to file, where logs will be written.
     *        threadId - id of thread, which logs will be written in filePath.
     *                   By default - thread, which called this method.
     */
    static bool AddLogFile(const std::filesystem::path &filePath, const std::thread::id &threadId=std::this_thread::get_id());

    /* @brief: Changes logfile for thread.
     * @param:
     *        filePath - new logfile.
     *        threadId - tid of thread, which will be assiciated with this log
     */
    static bool ChangeLogFile(const std::filesystem::path &filePath, const std::thread::id &threadId);


    /* @brief: shows, all info about logfiles.
    */
    static void ShowLogFiles();

    /* @brief: creates Log object and adds it to queue.
     */
    static void log(const std::string& logMessage);

private:
    /* @brief: finds file associated with threadId and writes log in it.
     */
    static bool WriteLogMessageInFile(const std::string &logmsg, const std::thread::id &threadId);

    /* @brief: takes Log object from logStorage queue and writes it in file.
     *         It is running in separate thread!
     */
    static void ProcessMessages();

    /* @brief: reopens logfiles every day.
     *         It is running in separate thread!
     */
    static void ReopenLogFiles();

        static bool isInited;

    // associate thread, with logfile, where this thread should write log.
    static std::map<std::thread::id, std::filesystem::path> logFiles;

    // logfile pathes associated with file descriptors.
    static std::map<std::filesystem::path, std::ofstream> fds;

    static std::mutex mutex_;

    // mutex for working with logfiles.
    static std::mutex logFilemutex_;

    // is used to notify, that Log was added to logStorage. 
    static std::condition_variable conditionVariable_;
    
    // queue, which stores logs, so that ProcessMessages thread could get logs from it and write in file.
    static std::queue<Log_t> logStorage;
};


#endif // LOGGER_HPP