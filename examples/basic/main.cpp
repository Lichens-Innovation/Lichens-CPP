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

/**
 * @brief Prints usage and available command-line options to standard output.
 *
 * Prints the program name followed by a short list of supported options:
 * `--help`, `--version`, and `--message <msg>`.
 *
 * @param app_name Name of the executable (typically `argv[0]`) used in the usage line.
 */
void print_help(const std::string& app_name)
{
    std::cout << "Usage: " << app_name << " [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --help                   Show this help message\n";
    std::cout << "  --version                Show version information\n";
    std::cout << "  --message <msg>          Log a message to the logger\n";
}

/**
 * @brief Configures and initializes the global logger for the application.
 *
 * Adds a console logger at TRACE level, adds a file logger that writes to ./demo_app.log at WARNING level,
 * and initializes the logger with the name "demo_app".
 */
void init_logger()
{
    LichensCPP::Logger::instance()
        .add_console_logger(LichensCPP::LogLevel::TRACE)
        .add_file_logger("./", "demo_app.log", LichensCPP::LogLevel::WARNING)
        .init("demo_app");
}

/**
 * @brief Program entry point that initializes logging, processes command-line options, logs messages, and exits.
 *
 * This function sets up the application's logger, emits startup and shutdown warnings, and handles the following
 * command-line options:
 * - `--message <msg>`: logs `<msg>` at INFO level.
 * - `--version`: prints the application version and exits.
 * - `--help` or `-h`: prints usage information and exits.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings; argv[0] is the program name used for help output.
 * @return int `0` on successful completion (including after handling `--version` or `--help`). 
 */
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