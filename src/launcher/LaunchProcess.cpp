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

    std::string LaunchProcess::shell_escape(const std::string &arg)
    {
        std::string escaped = "'";
        for (char c : arg)
        {
            if (c == '\'')
            {
                escaped += "'\\''"; // escape single quote
            }
            else
            {
                escaped += c;
            }
        }
        escaped += "'";
        return escaped;
    }

    std::string LaunchProcess::build_command(const std::vector<std::string> &args)
    {
        std::string cmd;
        for (const auto &arg : args)
        {
            if (!cmd.empty())
            {
                cmd += " ";
            }
            cmd += shell_escape(arg);
        }
        return cmd;
    }

    LaunchProcess::LaunchProcess(const std::string &command, const std::vector<std::string> &args, bool detached)
        : m_detached(detached), m_command(command), m_args(args), m_pid(-1)
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

        // Check for zombie process and reap it if needed
        int status = 0;
        pid_t result = waitpid(m_pid, &status, WNOHANG);

        if (result == m_pid)
        {
            // Child has exited — reap completed
            return false;
        }
        else if (result == -1)
        {
            switch (errno)
            {
            case ECHILD:
            {
                // No child — already reaped or never existed
                return false;
            }
            case EINTR:
            {
                // Interrupted by signal — harmless, just try again later
                LOG_TRACE_S("waitpid interrupted by signal for pid: ", m_pid);
                return true; // treat as still running (we don't know yet)
            }
            case EINVAL:
            case EFAULT:
            default:
            {
                LOG_WARNING_S("waitpid failed for pid: ", m_pid, " errno: ", errno);
                return false;
            }
            }
        }
        else if (result == 0)
        {
            // No change in child state — still alive or zombie
            if (kill(m_pid, 0) == 0)
            {
                return true; // still running
            }

            if (errno == EPERM)
            {
                LOG_WARNING_S("Process ", m_pid, " exists but permission denied to signal");
                return true;
            }

            // ESRCH: no such process
            return false;
        }

        LOG_WARNING_S("waitpid failed for pid: ", m_pid, " errno: ", errno);
        return false;
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

    bool LaunchProcess::launch_attached()
    {
        // Launch the process no matter what
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

    bool LaunchProcess::launch_detached()
    {
        // Launch the process no matter what
        m_pid = -1;
        std::vector<const char *> argv;
        argv.reserve(4u);

        // Make sure environment is loaded for detached processes
        const std::string shell = "/bin/bash";
        argv.emplace_back(shell.c_str());
        argv.emplace_back("-lc");
        std::string full_command = shell_escape(m_command) + " " + build_command(m_args);
        argv.emplace_back(full_command.c_str());
        argv.emplace_back(nullptr); // argv must be null-terminated

        posix_spawnattr_t attr;
        posix_spawnattr_init(&attr);

        posix_spawn_file_actions_t file_actions;
        posix_spawn_file_actions_init(&file_actions);
        if (m_detached)
        {
            // Detach from terminal and prevent open IO to carry over
            posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
            posix_spawn_file_actions_addclose(&file_actions, 0);
            posix_spawn_file_actions_addclose(&file_actions, 1);
            posix_spawn_file_actions_addclose(&file_actions, 2);
        }
        return posix_spawn(&m_pid, shell.c_str(), &file_actions, &attr, const_cast<char *const *>(argv.data()), nullptr) == 0;
    }

    bool LaunchProcess::launch()
    {
        if (m_detached)
        {
            return launch_detached();
        }
        return launch_attached();
    }

    RelaunchStatus LaunchProcess::relaunch_if_not_running()
    {
        if (!is_running())
        {
            LOG_TRACE_S("Process not running: ", m_command);
            if (m_pid != -1)
            {
                LOG_WARNING_S("Process with PID ", m_pid, " is dead, relaunching: ", m_command);
            }
            else
            {
                LOG_WARNING_S("Process not started yet: ", m_command);
            }
            if (!launch())
            {
                LOG_ERROR_S("Failed to relaunch process: ", m_command);
                return RelaunchStatus::Failed;
            }
            return RelaunchStatus::Relaunched;
        }
        else
        {
            LOG_TRACE_S("Process is still running: ", m_command, " pid: ", m_pid);
        }
        return RelaunchStatus::Running;
    }

} // namespace LichensCPP
