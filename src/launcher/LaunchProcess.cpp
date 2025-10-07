/*
 * Created on Wed Sep 10 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/launcher/LaunchProcess.h"

#include <cerrno>
#include <csignal>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "LichensCPP/log/LoggerHelper.h"

namespace LichensCPP
{

LaunchProcess::LaunchProcess(const std::string& command, const std::vector<std::string>& args)
    : m_command(command)
    , m_args(args)
    , m_pid(-1)
{
}

LaunchProcess::~LaunchProcess()
{
    terminate();
}

pid_t LaunchProcess::pid() const
{ 
    return m_pid;
}

bool LaunchProcess::is_running() const
{
    if (m_pid == -1)
    {
        return false;
    }

    // Check if process is running
    if (kill(m_pid, 0) == 0)
    {
        return true;
    }
    if (errno == EPERM) 
    {
        LOG_WARNING_S("Process ", m_pid, " exists but no permission to signal");
        return true;  // exists but no permission to signal
    }
    return false; // errno == ESRCH
}

bool LaunchProcess::terminate()
{
    if (m_pid == -1)
    {
        return true;
    }

    if (!is_running())
    {
        m_pid = -1;
        return true; // Process not running
    }

    if (kill(m_pid, SIGTERM) == 0)
    {
        // wait for process to terminate
        int status;
        waitpid(m_pid, &status, 0);
        m_pid = -1;
        return true;
    }
    // Failed to send signal, keep pid until we can
    return false;
}

bool LaunchProcess::launch()
{
    // Launch the process not matter what
    m_pid = -1;

    std::vector<const char *> argv;
    argv.reserve(m_args.size() + 2u);
    argv.emplace_back(m_command.c_str());
    for (const auto &arg : m_args)
    {
        argv.emplace_back(arg.c_str());
    }
    argv.emplace_back(nullptr); // argv must be null-terminated

    return posix_spawn(&m_pid, m_command.c_str(), nullptr, nullptr, const_cast<char *const *>(argv.data()), nullptr) == 0;
}

bool LaunchProcess::relaunch_if_not_running()
{
    if (!is_running())
    {
        if(!launch())
        {
            LOG_ERROR_S("Failed to relaunch process: ", m_command);
            return false;
        }
    }
    return true;
}

} // namespace LichensCPP