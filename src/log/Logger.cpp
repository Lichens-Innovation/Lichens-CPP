/*
 * Created on Mon Sep 08 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */
#include "LichensCPP/log/Logger.h"
#include "LichensCPP/filesystem/filesystem.h"

#include <iostream>
#include <mutex>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifndef _WIN32
    #include <spdlog/sinks/syslog_sink.h>
#endif

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
    std::mutex log_mutex;
    std::vector<spdlog::sink_ptr> sinks;
    bool syslog_unsupported_requested = false;

    static constexpr const char* syslog_unsupported_message =
        "Syslog logger is not supported on this platform, it is ignored";

    spdlog::level::level_enum toSpdLevel(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::Trace:   return spdlog::level::trace;
            case LogLevel::Debug:   return spdlog::level::debug;
            case LogLevel::Info:    return spdlog::level::info;
            case LogLevel::Warning: return spdlog::level::warn;
            case LogLevel::Error:   return spdlog::level::err;
            case LogLevel::Fatal:   return spdlog::level::critical;
        };
        return spdlog::level::info; // Default case
    }

    LoggerPrivate()
    {
    }

    void add_console_logger(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(toSpdLevel(level));
        sinks.push_back(console_sink);
    }

    #ifdef BUILD_LICHENS_CPP_FILESYSTEM
    void add_file_logger(const std::string& folder, const std::string& filename, LogLevel level)
    {
        const std::string log_file_path = std::string(folder) + "/" + std::string(filename);
        std::filesystem::create_directories(folder);

        std::lock_guard<std::mutex> lock(log_mutex);
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            log_file_path, PLAZ_LOGGER_MAX_FILE_SIZE, PLAZ_LOGGER_MAX_FILES);
        file_sink->set_level(toSpdLevel(level));
        sinks.push_back(file_sink);
    }
    #endif

    void add_syslog_logger(const std::string& logger_name, LogLevel level)
    {
        std::lock_guard<std::mutex> lock(log_mutex);
#ifdef _WIN32
        #pragma message("Windows systlog not implemented yet.")
        (void)logger_name;
        (void)level;
        if (logger)
        {
            logger->warn(syslog_unsupported_message);
        }
        else
        {
            syslog_unsupported_requested = true;
        }
#else
        // TODO godboutj 2025-09-08, check if we should LOG_DAEMON instead of LOG_USER
        auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>(logger_name, 0, LOG_USER, true);
        syslog_sink->set_level(toSpdLevel(level));
        sinks.push_back(syslog_sink);
#endif
    }

    void init(const std::string& logger_name)
    {
        logger = std::make_shared<spdlog::logger>(logger_name, begin(sinks), end(sinks));
        spdlog::register_logger(logger);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
        logger->set_level(spdlog::level::trace); // Capture all levels; individual sinks filter their own levels
        logger->flush_on(spdlog::level::err); // Flush on error level and above
        if (syslog_unsupported_requested)
        {
            logger->warn(syslog_unsupported_message);
        }
    }

    void shutdown()
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        spdlog::shutdown(); // This flushes and cleans up all loggers, do not log anything after this
    }

    void log(const LogLevel level, const std::string& message)
    {
        logger->log(toSpdLevel(level), message);
    }
    
    void flush_all()
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        logger->flush();
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

#ifdef BUILD_LICHENS_CPP_FILESYSTEM
Logger& Logger::add_file_logger(const std::string& folder, const std::string& filename, LogLevel level)
{
    p_->add_file_logger(folder, filename, level);
    return *this;
}
#endif

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

void Logger::shutdown()
{
    p_->shutdown();
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

void Logger::flush_all()
{
    p_->flush_all();
}

} // namespace LichensCPP
