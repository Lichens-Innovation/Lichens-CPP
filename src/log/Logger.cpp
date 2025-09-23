/*
 * Created on Mon Sep 08 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */
#include "LichensCPP/log/Logger.h"

#include <filesystem>
#include <iostream>
#include <mutex>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

#ifndef PLAZ_LOGGER_MAX_FILE_SIZE
    #define PLAZ_LOGGER_MAX_FILE_SIZE (5u * 1024u * 1024u)
#endif
#ifndef PLAZ_LOGGER_MAX_FILES
    #define PLAZ_LOGGER_MAX_FILES 3
#endif

namespace LichensCPP
{
struct Logger::LoggerPrivate
{
    std::shared_ptr<spdlog::logger> logger;
    std::mutex logMutex;
    std::vector<spdlog::sink_ptr> sinks;

    spdlog::level::level_enum toSpdLevel(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::TRACE:   return spdlog::level::trace;
            case LogLevel::DEBUG:   return spdlog::level::debug;
            case LogLevel::INFO:    return spdlog::level::info;
            case LogLevel::WARNING: return spdlog::level::warn;
            case LogLevel::ERROR:   return spdlog::level::err;
            case LogLevel::FATAL:   return spdlog::level::critical;
        };
        return spdlog::level::info; // Default case
    }

    LoggerPrivate()
    {
    }

    void add_console_logger(LogLevel level)
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(toSpdLevel(level));
        sinks.push_back(console_sink);
    }

    void add_file_logger(const std::string& folder, const std::string& filename, LogLevel level)
    {
        const std::string log_file_path = std::string(folder) + "/" + std::string(filename);
        std::filesystem::create_directories(folder);
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            log_file_path, PLAZ_LOGGER_MAX_FILE_SIZE, PLAZ_LOGGER_MAX_FILES);
        file_sink->set_level(toSpdLevel(level));
        sinks.push_back(file_sink);
    }

    void add_syslog_logger(const std::string& logger_name, LogLevel level)
    {
        // TODO godboutj 2025-09-08, check if we should LOG_DAEMON instead of LOG_USER
        auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>(logger_name, 0, LOG_USER, true);
        syslog_sink->set_level(toSpdLevel(level));
        sinks.push_back(syslog_sink);
    }

    void init(const std::string& logger_name)
    {
        logger = std::make_shared<spdlog::logger>(logger_name, begin(sinks), end(sinks));
        spdlog::register_logger(logger);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
        logger->set_level(spdlog::level::trace); // Capture all levels; individual sinks filter their own levels
        logger->flush_on(spdlog::level::err); // Flush on error level and above
    }

    void log(const LogLevel level, const std::string& message)
    {
        logger->log(toSpdLevel(level), message);
    }   
};

Logger::Logger()
    : p_(std::make_unique<LoggerPrivate>())
{
}

Logger::~Logger()
{
}

Logger& Logger::add_console_logger(LogLevel level)
{
    p_->add_console_logger(level);
    return *this;
}

Logger& Logger::add_file_logger(const std::string& folder, const std::string& filename, LogLevel level)
{
    p_->add_file_logger(folder, filename, level);
    return *this;
}

Logger& Logger::add_syslog_logger(const std::string& logger_name, LogLevel level)
{
    p_->add_syslog_logger(logger_name, level);
    return *this;
}

Logger& Logger::init(const std::string& logger_name)
{
    p_->init(logger_name);
    return *this;
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const LogLevel level, const std::string& message)
{
    p_->log(level, message);
}

} // namespace LichensCPP