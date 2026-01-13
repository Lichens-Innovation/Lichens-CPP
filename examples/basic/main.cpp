/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-01-13
 */

#include <iostream>
#include <thread>

#include <LichensCPP/log/Logger.h>
#include <LichensCPP/log/LoggerHelper.h>

void print_help(const std::string& app_name)
{
    std::cout << "Usage: " << app_name << " [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --help                   Show this help message\n";
    std::cout << "  --version                Show version information\n";
    std::cout << "  --message <msg>          Log a message to the logger\n";
}

void init_logger()
{
    LichensCPP::Logger::instance()
        .add_console_logger(LichensCPP::LogLevel::TRACE)
        .add_file_logger("./", "demo_app.log", LichensCPP::LogLevel::WARNING)
        .init("demo_app");
}

int main(int argc, char* argv[])
{
    init_logger();
    LOG_WARNING("**** Demo app starting");
    LOG_FLUSH();

    for (int i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--message" && i + 1 < argc)
        {
            LOG_INFO(argv[++i]);
        }
        else if(std::string(argv[i]) == "--version")
        {
            std::cout << "Demo App version 1.0.0\n";
            return 0;
        }
        else if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
        {
            print_help(argv[0]);
            return 0;
        }
    }

    LOG_ERROR("This is an error message");

    LOG_WARNING("**** Demo app quitting");
    return 0;
}
