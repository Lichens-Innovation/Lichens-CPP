/*
 * Created on Mon Nov 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/launcher/MainLoopHelper.h"

#include <chrono>
#include <condition_variable>
#include <csignal>
#include <thread>
#include <atomic>

#include "LichensCPP/log/LoggerHelper.h"

std::atomic<bool> running{true};
std::condition_variable cv;
std::mutex mtx;

namespace LichensCPP
{

    struct MainLoopHelper::MainLoopHelperPrivate
    {
        std::chrono::milliseconds m_interval;

        MainLoopHelperPrivate(const std::chrono::milliseconds &interval)
            : m_interval(interval)
        {
        }
    };

    MainLoopHelper::MainLoopHelper(const std::chrono::milliseconds &interval)
        : p_(std::make_unique<MainLoopHelperPrivate>(interval))
    {
        std::signal(
            SIGINT,
            [](int)
            {
                LOG_INFO("Received SIGINT, shutting down...");
                running = false;
                cv.notify_all();
            });
        std::signal(
            SIGTERM,
            [](int)
            {
                LOG_INFO("Received SIGTERM, shutting down...");
                running = false;
                cv.notify_all();
            });
    }

    MainLoopHelper::~MainLoopHelper()
    {
        stop();
    }

    void MainLoopHelper::run(const std::function<void()> &task)
    {
        std::unique_lock<std::mutex> lock(mtx);

        while (running)
        {
            task();
            cv.wait_for(lock, m_interval);
        }
    }

    void MainLoopHelper::notify()
    {
        cv.notify_all();
    }

    void MainLoopHelper::stop()
    {
        if (!running)
        {
            return; // Not running
        }
        running = false;
        cv.notify_all();
    }

} // namespace LichensCPP
