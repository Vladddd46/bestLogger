#include "Logger.hpp"

int main() {
    Logger::Init();


    std::thread thread1([](){
        Logger::AddLogFile("logfile1.txt");
        Logger::Log("Hello from Thread 1 - Message 1");
        Logger::Log("Hello from Thread 1 - Message 2");
    });

    std::thread thread2([]() {
        Logger::AddLogFile("logfile2.txt");
        Logger::Log("Hello from Thread 2 - Message 1");
        Logger::Log("Hello from Thread 2 - Message 2");
    });

    std::thread thread3([]() {
        Logger::AddLogFile("logfile2.txt");
        Logger::Log("Hello from Thread 3 - Message 1");
        Logger::Log("Hello from Thread 3 - Message 2");
    });

    std::thread thread4([]() {
        Logger::AddLogFile("logfile2.txt");
        Logger::Log("Hello from Thread 4 - Message 1");
        Logger::Log("Hello from Thread 4 - Message 2");
    });

    std::thread thread5([]() {
        Logger::AddLogFile("logfile2.txt");
        Logger::Log("Hello from Thread 5- Message 1");
        Logger::Log("Hello from Thread 5 - Message 2");
    });

    std::thread thread6([]() {
        Logger::AddLogFile("logfile2.txt");
        Logger::Log("Hello from Thread 6- Message 1");
        Logger::Log("Hello from Thread 6 - Message 2");
    });

    Logger::Log("Hello from main thread");
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();

    while (1) {}
    Logger::Deinit();
    return 0;
}
