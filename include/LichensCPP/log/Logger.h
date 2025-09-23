/*
 * Created on Mon Sep 08 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <memory>
#include <string>

namespace LichensCPP
{

enum class LogLevel
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger
{
public:
    Logger();
    ~Logger();

    Logger& add_console_logger(LogLevel level);
    Logger& add_file_logger(const std::string& folder, const std::string& filename, LogLevel level);
    Logger& add_syslog_logger(const std::string& logger_name, LogLevel level);

    // Must call the init after the add_xxx_logger methods but before any log()
    Logger& init(const std::string& logger_name);

    static Logger& instance();
    void log(const LogLevel level, const std::string& message);
    
private:
    struct LoggerPrivate;
    std::unique_ptr<LoggerPrivate> p_;
};

} // namespace LichensCPP