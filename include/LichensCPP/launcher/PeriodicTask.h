/*
 * Created on Tue Sep 30 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>

namespace LichensCPP
{

class PeriodicTask
{
public:
    PeriodicTask(std::chrono::milliseconds interval, std::function<bool()> callback);
    ~PeriodicTask();

    PeriodicTask(const PeriodicTask&) = delete;
    PeriodicTask& operator=(const PeriodicTask&) = delete;

    void start();
    void stop();
    bool is_running() const;

private:
    struct PeriodicTaskPrivate;
    std::unique_ptr<PeriodicTaskPrivate> p_;
};

} // namespace LichensCPP