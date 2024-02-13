#include "Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>

Logger::Logger()
{
    // Get the current working directory
    const std::filesystem::path currentPath = std::filesystem::current_path();

    // Get the current date
    const std::time_t time = std::time(nullptr);
    const std::tm* timeInfo = std::localtime(&time);

    // Get the date as a string
    const std::string dateString = std::to_string(timeInfo->tm_year + 1900) + "-" +
        std::to_string(timeInfo->tm_mon + 1) + "-" +
        std::to_string(timeInfo->tm_mday);

    // Create the log in the logs directory, creating the directory if it does not exist
    create_directory(currentPath / "logs");

    // Construct the path
    filepath_ = std::filesystem::path(currentPath / "logs" / "VulkanRenderer_");
    filepath_ += dateString + ".log";

    // Create the log file if it does not already exist.
    if (!exists(filepath_))
    {
        std::ofstream file(filepath_);
        file.close();
    }
}

void Logger::LogMessage(const std::string& Message)
{
    // Lock the mutex
    std::lock_guard lock(logMutex_);

    const auto now = std::chrono::system_clock::now();
    const auto nowT = std::chrono::system_clock::to_time_t(now);
    const std::tm* nowTm = std::localtime(&nowT);
    std::stringstream ss;
    ss << std::put_time(nowTm, "%H:%M:%S");

    const std::string msg = ss.str() + " | " + Message + '\n';
    
    // Open the file in append mode
    std::ofstream file(filepath_, std::ios_base::app);

    // Write the message to the file
    file << msg;

    // Close the file
    file.close();
    std::cout << msg;
    std::flush(std::cout);
}
