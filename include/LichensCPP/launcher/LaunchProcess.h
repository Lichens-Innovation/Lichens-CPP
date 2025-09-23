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

class LaunchProcess
{
public:
    LaunchProcess(const std::string& command, const std::vector<std::string>& args = {});
    ~LaunchProcess();

    pid_t pid() const;
    bool is_running() const;

    bool terminate();
    bool launch();

    bool relaunch_if_not_running();

private:
    std::string m_command;
    std::vector<std::string> m_args;
    std::vector<char*> m_argv;
    pid_t m_pid;
};

} // namespace LichensCPP