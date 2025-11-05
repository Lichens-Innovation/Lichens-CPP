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
#include <version>

#if __cplusplus >= 202002L
    #include <format>
#endif

#include "LichensCPP/log/Logger.h"

#ifndef LICHENS_LOG_BUFFER_SIZE
    #define LICHENS_LOG_BUFFER_SIZE 1024
#endif

namespace LichensCPP
{
#if __cplusplus < 202002L
    template <typename... Args>
    std::string log_str_format(const char *format, Args... args)
    {
        std::array<char, LICHENS_LOG_BUFFER_SIZE> buf;
        int written = std::snprintf(buf.data(), LICHENS_LOG_BUFFER_SIZE, format, args...);

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
    }
#endif

    template <typename... Args>
    std::string log_str_stream(Args... args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        return oss.str();
    }

} // namespace LichensCPP

#define LOG_FLUSH() LichensCPP::Logger::instance().flush_all()
#define LOG_SHUTDOWN() LichensCPP::Logger::instance().shutdown()

#define LOG(level, message) LichensCPP::Logger::instance().log(level, message)

#define LOG_TRACE(message)   LOG(LichensCPP::LogLevel::TRACE, message)
#define LOG_DEBUG(message)   LOG(LichensCPP::LogLevel::DEBUG, message)
#define LOG_INFO(message)    LOG(LichensCPP::LogLevel::INFO, message)
#define LOG_WARNING(message) LOG(LichensCPP::LogLevel::WARNING, message)
#define LOG_ERROR(message)   LOG(LichensCPP::LogLevel::ERROR, message)
#define LOG_FATAL(message)   LOG(LichensCPP::LogLevel::FATAL, message)

#if __cplusplus >= 202002L
    // C++20 and newer
    #define LOG_TRACE_F(...)   LOG_TRACE(  std::format(__VA_ARGS__))
    #define LOG_DEBUG_F(...)   LOG_DEBUG(  std::format(__VA_ARGS__))
    #define LOG_INFO_F(...)    LOG_INFO(   std::format(__VA_ARGS__))
    #define LOG_WARNING_F(...) LOG_WARNING(std::format(__VA_ARGS__))
    #define LOG_ERROR_F(...)   LOG_ERROR(  std::format(__VA_ARGS__))
    #define LOG_FATAL_F(...)   LOG_FATAL(  std::format(__VA_ARGS__))
#else
    // C++ 17 and lower, might need to reduce warning level to avoid empty __VA_ARGS__ warning
    #define LOG_TRACE_F(format, ...)   LOG_TRACE(  LichensCPP::log_str_format(format, ##__VA_ARGS__))
    #define LOG_DEBUG_F(format, ...)   LOG_DEBUG(  LichensCPP::log_str_format(format, ##__VA_ARGS__))
    #define LOG_INFO_F(format, ...)    LOG_INFO(   LichensCPP::log_str_format(format, ##__VA_ARGS__))
    #define LOG_WARNING_F(format, ...) LOG_WARNING(LichensCPP::log_str_format(format, ##__VA_ARGS__))
    #define LOG_ERROR_F(format, ...)   LOG_ERROR(  LichensCPP::log_str_format(format, ##__VA_ARGS__))
    #define LOG_FATAL_F(format, ...)   LOG_FATAL(  LichensCPP::log_str_format(format, ##__VA_ARGS__))
#endif

#define LOG_TRACE_S(...)   LOG_TRACE(  LichensCPP::log_str_stream(__VA_ARGS__))
#define LOG_DEBUG_S(...)   LOG_DEBUG(  LichensCPP::log_str_stream(__VA_ARGS__))
#define LOG_INFO_S(...)    LOG_INFO(   LichensCPP::log_str_stream(__VA_ARGS__))
#define LOG_WARNING_S(...) LOG_WARNING(LichensCPP::log_str_stream(__VA_ARGS__))
#define LOG_ERROR_S(...)   LOG_ERROR(  LichensCPP::log_str_stream(__VA_ARGS__))
#define LOG_FATAL_S(...)   LOG_FATAL(  LichensCPP::log_str_stream(__VA_ARGS__))
