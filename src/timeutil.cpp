/*
 * Created on Wed Nov 12 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/timeutil.h"

#include <chrono>

namespace LichensCPP
{
    uint64_t time_now()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

    uint64_t time_delta_past(uint64_t past_reference, uint64_t t_relative)
    {
        if (t_relative >= past_reference)
        {
            return t_relative - past_reference;
        }
        return 0u;
    }

    int64_t time_delta(uint64_t t_reference, uint64_t t_relative)
    {
        if (t_relative >= t_reference)
        {
            return static_cast<int64_t>(t_relative - t_reference);
        }
        return -static_cast<int64_t>(t_reference - t_relative);
    }

    uint64_t time_delta_past_now(uint64_t past_timestamp)
    {
        uint64_t now_time = time_now();
        if (now_time >= past_timestamp)
        {
            return now_time - past_timestamp;
        }
        return 0u;
    }

    int64_t time_delta_now(uint64_t past_timestamp)
    {
        return time_delta(past_timestamp, time_now());
    }

    float time_sec(uint64_t t)
    {
        return static_cast<float>(t) / 1e9f;
    }

    float time_msec(uint64_t t)
    {
        return static_cast<float>(t) / 1e6f;
    }

    float time_usec(uint64_t t)
    {
        return static_cast<float>(t) / 1e3f;
    }

    float time_nsec(uint64_t t)
    {
        return static_cast<float>(t);
    }

    uint64_t from_sec(float seconds)
    {
        if (!(seconds > 0.0f))
        {
            return 0u;
        }
        return static_cast<uint64_t>(seconds * 1e9f);
    }

    uint64_t from_msec(float milliseconds)
    {
        if (!(milliseconds > 0.0f))
        {
            return 0u;
        }
        return static_cast<uint64_t>(milliseconds * 1e6f);
    }

    uint64_t from_usec(float microseconds)
    {
        if (!(microseconds > 0.0f))
        {
            return 0u;
        }
        return static_cast<uint64_t>(microseconds * 1e3f);
    }

    uint64_t from_nsec(float nanoseconds)
    {
        if (!(nanoseconds > 0.0f))
        {
            return 0u;
        }
        return static_cast<uint64_t>(nanoseconds);
    }
    }

    uint64_t frequency_to_period(float frequency)
    {
        if (frequency <= 0.0f)
        {
            return 0u;
        }
        return static_cast<uint64_t>(1e9f / frequency);
    }

} // namespace LichensCPP