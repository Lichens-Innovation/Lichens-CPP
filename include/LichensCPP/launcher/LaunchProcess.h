/*
 * Created on Wed Sep 10 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

namespace LichensCPP
{

    enum class RelaunchStatus
    {
        Running = 0,
        Relaunched,
        Failed
    };

    class LaunchProcess
    {
    public:
        LaunchProcess(const std::string &command, const std::vector<std::string> &args = {}, bool detached = false);
        ~LaunchProcess();

        pid_t pid() const;
        bool is_running() const;

        bool terminate();
        bool launch();

        RelaunchStatus relaunch_if_not_running();

        static std::string shell_escape(const std::string &arg);
        static std::string build_command(const std::vector<std::string> &args);

    private:
        bool launch_attached();
        bool launch_detached();

        const bool m_detached;
        const std::string m_command;
        const std::vector<std::string> m_args;
        pid_t m_pid;
    };

} // namespace LichensCPP
