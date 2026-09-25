/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-09-25
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "LichensCPP/log/Logger.h"

using namespace LichensCPP;

namespace
{
    std::string read_file(const std::filesystem::path &path)
    {
        std::ifstream file(path);
        return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }

    // Logs one message per level to a file sink filtered at sink_level, returns the file content
    std::string log_all_levels_to_file(const std::string &logger_name, LogLevel sink_level)
    {
        const auto folder = std::filesystem::temp_directory_path() / "lichens_cpp_tests";
        const std::string filename = logger_name + ".log";
        std::error_code ec;
        std::filesystem::remove(folder / filename, ec);

        Logger logger;
        logger.add_file_logger(folder.string(), filename, sink_level).init(logger_name);
        logger.log(LogLevel::Trace, "trace message");
        logger.log(LogLevel::Debug, "debug message");
        logger.log(LogLevel::Info, "info message");
        logger.log(LogLevel::Warning, "warning message");
        logger.log(LogLevel::Error, "error message");
        logger.log(LogLevel::Fatal, "fatal message");
        logger.flush_all();

        return read_file(folder / filename);
    }
} // namespace

TEST(LoggerTest, newLevelNamesMapToMatchingSpdlogLevels)
{
    const std::string content = log_all_levels_to_file("logger_test_all_levels", LogLevel::Trace);

    EXPECT_NE(content.find("[trace] trace message"), std::string::npos);
    EXPECT_NE(content.find("[debug] debug message"), std::string::npos);
    EXPECT_NE(content.find("[info] info message"), std::string::npos);
    EXPECT_NE(content.find("[warning] warning message"), std::string::npos);
    EXPECT_NE(content.find("[error] error message"), std::string::npos);
    EXPECT_NE(content.find("[critical] fatal message"), std::string::npos);
}

TEST(LoggerTest, sinkLevelFiltersLowerLevels)
{
    const std::string content = log_all_levels_to_file("logger_test_warning_level", LogLevel::Warning);

    EXPECT_EQ(content.find("trace message"), std::string::npos);
    EXPECT_EQ(content.find("debug message"), std::string::npos);
    EXPECT_EQ(content.find("info message"), std::string::npos);
    EXPECT_NE(content.find("warning message"), std::string::npos);
    EXPECT_NE(content.find("error message"), std::string::npos);
    EXPECT_NE(content.find("fatal message"), std::string::npos);
}

#ifndef _WIN32
// The uppercase names are deprecated, silence the warning to check they still work
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
TEST(LoggerTest, deprecatedUppercaseNamesEqualNewNames)
{
    EXPECT_EQ(LogLevel::TRACE, LogLevel::Trace);
    EXPECT_EQ(LogLevel::DEBUG, LogLevel::Debug);
    EXPECT_EQ(LogLevel::INFO, LogLevel::Info);
    EXPECT_EQ(LogLevel::WARNING, LogLevel::Warning);
    EXPECT_EQ(LogLevel::ERROR, LogLevel::Error);
    EXPECT_EQ(LogLevel::FATAL, LogLevel::Fatal);
}
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
#endif
