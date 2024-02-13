#pragma once
#include <filesystem>
#include <mutex>
#include <string>

enum LogCategory
{
    Log,
    Debug,
    Warning,
    Error,
    Fatal
};

#define CUR_LINE() __LINE__
#define CUR_FUNC() __func__
#define CUR_FILE() __FILE__

#define MESSAGE_BODY(msg) \
    std::string(CUR_FILE()) + ":" + std::string(CUR_FUNC()) + \
    "(" + std::to_string(CUR_LINE()) + ")" + \
    " " + (std::string(msg))

#define G_LOG(logger, category, msg) \
    { \
        std::string categoryStr; \
        switch (category) \
        { \
        case LogCategory::Log: \
            categoryStr = "Log      "; \
            break; \
        case LogCategory::Debug: \
            categoryStr = "Debug    "; \
            break; \
        case LogCategory::Warning: \
            categoryStr = "Warning  "; \
            break; \
        case LogCategory::Error: \
            categoryStr = "Error    "; \
            break; \
        case LogCategory::Fatal: \
            categoryStr = "Fatal    "; \
            break; \
        default: \
            categoryStr = "Log      "; \
            break; \
        } \
        std::thread t(&Logger::LogMessage, logger, categoryStr + "| " + MESSAGE_BODY(msg)); \
        t.detach(); \
    }
    

class Logger
{
private:
    std::filesystem::path filepath_;

    std::mutex logMutex_;
    
public:
    Logger();

    void LogMessage(const std::string& Message);
};
