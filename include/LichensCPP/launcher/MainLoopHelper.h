/*
 * Created on Mon Nov 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>

namespace LichensCPP
{
    class MainLoopHelper
    {
    public:
        MainLoopHelper(const std::chrono::milliseconds &interval);
        ~MainLoopHelper();

        void run(const std::function<void()> &task);
        void notify();
        void stop();

    private:
        struct MainLoopHelperPrivate;
        std::unique_ptr<struct MainLoopHelperPrivate> p_;
    };

} // namespace LichensCPP
