#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>

namespace utils 
{
    bool createFile(const std::filesystem::path &filePath);

    std::string getCurrentDateTime();

    std::string getCurrentDate();

} // end of utils
#endif // UTILS_HPP