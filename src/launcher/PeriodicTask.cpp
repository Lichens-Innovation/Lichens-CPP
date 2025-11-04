/*
 * Created on Tue Sep 30 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/launcher/PeriodicTask.h"

#include <thread>

#include "LichensCPP/log/LoggerHelper.h"

namespace LichensCPP
{

struct PeriodicTask::PeriodicTaskPrivate
{
    std::chrono::milliseconds m_interval;
    std::function<bool()> m_task;
    std::unique_ptr<std::thread> m_worker_thread;
    bool m_running;

    PeriodicTaskPrivate(std::chrono::milliseconds interval, std::function<bool()> callback)
        : m_task(callback), m_interval(interval), m_worker_thread(), m_running(false)
    {
    }

    ~PeriodicTaskPrivate()
    {
        stop();
    }

    void stop()
    {
        if(m_running)
        {
            m_running = false;
            if(m_worker_thread && m_worker_thread->joinable())
            {
                m_worker_thread->join();
            }
        }
        m_worker_thread.reset();
    }

    void start()
    {
        if(m_running)
        {
            return;
        }

        m_running = true;
        m_worker_thread = std::make_unique<std::thread>([this]()
        {
            while(m_running)
            {
                try
                {
                    if(!m_task())
                    {
                        m_running = false;
                        LOG_INFO("Periodic task requested to stop");
                        break;
                    }
                }
                catch(const std::exception& e)
                {
                    LOG_ERROR_S("Periodic task threw an exception: ", e.what());
                }

                std::this_thread::sleep_for(m_interval);
            }
        });
    }
};


PeriodicTask::PeriodicTask(std::chrono::milliseconds interval, std::function<bool()> task)
    : p_(std::make_unique<PeriodicTaskPrivate>(interval, task))
{
}

PeriodicTask::~PeriodicTask()
{
    stop();
}


void PeriodicTask::start()
{
    p_->start();
}

void PeriodicTask::stop()
{
    p_->stop();
}

bool PeriodicTask::is_running() const
{
    return p_->m_running;
}

} // namespace LichensCPP