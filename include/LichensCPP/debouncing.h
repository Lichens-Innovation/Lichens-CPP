/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-01-22
 */

#pragma once

#include <cstdint>
#include <functional>
#include <utility>
#include <optional>

#include "LichensCPP/log/LoggerHelper.h"
#include "LichensCPP/timeutil.h"

namespace LichensCPP
{
    template <typename CallbackType>
    class Debouncing
    {
    public:
        Debouncing(CallbackType callback, uint64_t max_interval = 1e9, uint32_t max_nb_calls = 10u)
            : m_callback(std::move(callback)),
            m_max_interval(max_interval),
            m_max_nb_calls(max_nb_calls),
            m_last_timestamp(std::nullopt),
            m_nb_calls(0u)
        {
        }

        /* Args are forwarded to the debouncing callback */
        template <typename... Args>
        void call(Args&&... args)
        {
            ++m_nb_calls;

            if(!m_last_timestamp.has_value())
            {
                m_last_timestamp = time_now();
            }

            if (m_nb_calls >= m_max_nb_calls || time_delta_past_now(m_last_timestamp.value()) >= m_max_interval)
            {
                try
                {
                    m_callback(std::forward<Args>(args)...);
                }
                catch (std::exception& e)
                {
                    LOG_ERROR_S("Debouncing", "Exception in debounced callback: ", e.what());
                }

                m_nb_calls = 0u;
                m_last_timestamp = std::nullopt;
            }
        }

    private:
        CallbackType m_callback;
        uint64_t m_max_interval;
        uint32_t m_max_nb_calls;
        std::optional<uint64_t> m_last_timestamp;
        uint32_t m_nb_calls;
    };
} // namespace LichensCPP