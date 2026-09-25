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

// MSVC only reports the real language version in __cplusplus with /Zc:__cplusplus, _MSVC_LANG is always correct
#ifdef _MSVC_LANG
    #define LICHENS_CPP_CPLUSPLUS _MSVC_LANG
#else
    #define LICHENS_CPP_CPLUSPLUS __cplusplus
#endif

#if LICHENS_CPP_CPLUSPLUS >= 202002L
    #include <format>
#endif

#include "LichensCPP/log/Logger.h"
#include "LichensCPP/unused.h"

#ifndef LICHENS_LOG_BUFFER_SIZE
    #define LICHENS_LOG_BUFFER_SIZE 1024
#endif

#ifndef LICHENS_LOG_MIN_LEVEL
    #define LICHENS_LOG_MIN_LEVEL 0
#endif

namespace LichensCPP
{
#if LICHENS_CPP_CPLUSPLUS < 202002L
    // LOG_xxx_F("message") calls this with no args, the format is still interpreted as printf would
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-security"
#endif
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
            written = static_cast<int>(buf.size() - 1);
        }
        return std::string(buf.data(), buf.data() + written);
    }
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
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

#if LICHENS_LOG_MIN_LEVEL <= 0
    #define LOG_TRACE(message) LOG(LichensCPP::LogLevel::Trace, message)
#else
    #define LOG_TRACE(message) UNUSED(message)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 1
    #define LOG_DEBUG(message) LOG(LichensCPP::LogLevel::Debug, message)
#else
    #define LOG_DEBUG(message) UNUSED(message)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 2
    #define LOG_INFO(message) LOG(LichensCPP::LogLevel::Info, message)
#else
    #define LOG_INFO(message) UNUSED(message)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 3
    #define LOG_WARNING(message) LOG(LichensCPP::LogLevel::Warning, message)
#else
    #define LOG_WARNING(message) UNUSED(message)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 4
    #define LOG_ERROR(message) LOG(LichensCPP::LogLevel::Error, message)
#else
    #define LOG_ERROR(message) UNUSED(message)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 5
    #define LOG_FATAL(message) LOG(LichensCPP::LogLevel::Fatal, message)
#else
    #define LOG_FATAL(message) UNUSED(message)
#endif

#if LICHENS_CPP_CPLUSPLUS >= 202002L
    // C++20 and newer
    #if LICHENS_LOG_MIN_LEVEL <= 0 
        #define LOG_TRACE_F(...)   LOG_TRACE(  std::format(__VA_ARGS__))
    #else
        #define LOG_TRACE_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 1
        #define LOG_DEBUG_F(...)   LOG_DEBUG(  std::format(__VA_ARGS__))
    #else
        #define LOG_DEBUG_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 2
        #define LOG_INFO_F(...)    LOG_INFO(   std::format(__VA_ARGS__))
    #else
        #define LOG_INFO_F(...)    UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 3
        #define LOG_WARNING_F(...) LOG_WARNING(std::format(__VA_ARGS__))
    #else
        #define LOG_WARNING_F(...) UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 4
        #define LOG_ERROR_F(...)   LOG_ERROR(  std::format(__VA_ARGS__))
    #else
        #define LOG_ERROR_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 5
        #define LOG_FATAL_F(...)   LOG_FATAL(  std::format(__VA_ARGS__))
    #else
        #define LOG_FATAL_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
#else
    // C++ 17 and lower, the format string is the first of __VA_ARGS__
    #if LICHENS_LOG_MIN_LEVEL <= 0
        #define LOG_TRACE_F(...)   LOG_TRACE(  LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_TRACE_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 1
        #define LOG_DEBUG_F(...)   LOG_DEBUG(  LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_DEBUG_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 2
        #define LOG_INFO_F(...)    LOG_INFO(   LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_INFO_F(...)    UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 3
        #define LOG_WARNING_F(...) LOG_WARNING(LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_WARNING_F(...) UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 4
        #define LOG_ERROR_F(...)   LOG_ERROR(  LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_ERROR_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
    #if LICHENS_LOG_MIN_LEVEL <= 5
        #define LOG_FATAL_F(...)   LOG_FATAL(  LichensCPP::log_str_format(__VA_ARGS__))
    #else
        #define LOG_FATAL_F(...)   UNUSED_LIST(__VA_ARGS__)
    #endif
#endif

#if LICHENS_LOG_MIN_LEVEL <= 0
    #define LOG_TRACE_S(...)   LOG_TRACE(  LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_TRACE_S(...)   UNUSED_LIST(__VA_ARGS__)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 1
    #define LOG_DEBUG_S(...)   LOG_DEBUG(  LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_DEBUG_S(...)   UNUSED_LIST(__VA_ARGS__)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 2
    #define LOG_INFO_S(...)    LOG_INFO(   LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_INFO_S(...)    UNUSED_LIST(__VA_ARGS__)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 3
    #define LOG_WARNING_S(...) LOG_WARNING(LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_WARNING_S(...) UNUSED_LIST(__VA_ARGS__)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 4
    #define LOG_ERROR_S(...)   LOG_ERROR(  LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_ERROR_S(...)   UNUSED_LIST(__VA_ARGS__)
#endif
#if LICHENS_LOG_MIN_LEVEL <= 5
    #define LOG_FATAL_S(...)   LOG_FATAL(  LichensCPP::log_str_stream(__VA_ARGS__))
#else
    #define LOG_FATAL_S(...)   UNUSED_LIST(__VA_ARGS__)
#endif
