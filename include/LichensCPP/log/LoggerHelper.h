/*
 * Created on Mon Sep 08 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */
#pragma once

#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>

#include "LichensCPP/log/Logger.h"

#ifndef PLAZ_LOG_BUFFER_SIZE
    #define PLAZ_LOG_BUFFER_SIZE 1024
#endif

namespace LichensCPP
{

#define LOG_FLUSH() LichensCPP::Logger::instance().flush_all()
#define LOG_SHUTDOWN() LichensCPP::Logger::instance().shutdown()

#define LOG(level, message) LichensCPP::Logger::instance().log(level, message)

#define LOG_TRACE(message)   LOG(LichensCPP::LogLevel::TRACE,   message)
#define LOG_DEBUG(message)   LOG(LichensCPP::LogLevel::DEBUG,   message)
#define LOG_INFO(message)    LOG(LichensCPP::LogLevel::INFO,    message)
#define LOG_WARNING(message) LOG(LichensCPP::LogLevel::WARNING, message)
#define LOG_ERROR(message)   LOG(LichensCPP::LogLevel::ERROR,   message)
#define LOG_FATAL(message)   LOG(LichensCPP::LogLevel::FATAL,   message)

template<typename... Args>
std::string log_str_format(const char* format, Args... args)
{
    #if CPP_STANDARD >= 202002
        return std::format(format, std::forward<Args>(args)...);
    #else
        std::array<char, PLAZ_LOG_BUFFER_SIZE> buf;
        int written = std::snprintf(buf.data(), PLAZ_LOG_BUFFER_SIZE, format, args...);
        
        if (written < 0)
        {
            return "Log formatting error";
        }

        if (static_cast<size_t>(written) >= buf.size())
        {
            // Output was truncated; only buf.size()-1 chars written + '\0'
            written = buf.size() - 1;
        }
        return std::string(buf.data(), buf.data() + written);
    #endif
}

#define LOG_TRACE_F(format, ...)   LOG_TRACE(log_str_format(format, ##__VA_ARGS__))
#define LOG_DEBUG_F(format, ...)   LOG_DEBUG(log_str_format(format, ##__VA_ARGS__))
#define LOG_INFO_F(format, ...)    LOG_INFO(log_str_format(format, ##__VA_ARGS__))
#define LOG_WARNING_F(format, ...) LOG_WARNING(log_str_format(format, ##__VA_ARGS__))
#define LOG_ERROR_F(format, ...)   LOG_ERROR(log_str_format(format, ##__VA_ARGS__))
#define LOG_FATAL_F(format, ...)   LOG_FATAL(log_str_format(format, ##__VA_ARGS__))

template<typename... Args>
std::string log_str_stream(Args... args)
{
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

#define LOG_TRACE_S(...)   LOG_TRACE(log_str_stream(__VA_ARGS__))
#define LOG_DEBUG_S(...)   LOG_DEBUG(log_str_stream(__VA_ARGS__))
#define LOG_INFO_S(...)    LOG_INFO(log_str_stream(__VA_ARGS__))
#define LOG_WARNING_S(...) LOG_WARNING(log_str_stream(__VA_ARGS__))
#define LOG_ERROR_S(...)   LOG_ERROR(log_str_stream(__VA_ARGS__))
#define LOG_FATAL_S(...)   LOG_FATAL(log_str_stream(__VA_ARGS__))

} // namespace LichensCPP
