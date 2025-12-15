/*
 * Created on Wed Nov 12 2025
 *
 * Copyright (c) 2025 PointLAZ Inc. All rights reserved.
 */

#pragma once

#include <cstdint>

namespace LichensCPP
{
    uint64_t time_now();

    uint64_t time_delta_past(uint64_t past_reference, uint64_t t_relative);
    int64_t time_delta(uint64_t t_reference, uint64_t t_relative);

    uint64_t time_delta_past_now(uint64_t past_timestamp);
    int64_t time_delta_now(uint64_t past_timestamp);

    float time_sec(uint64_t t);
    float time_msec(uint64_t t);
    float time_usec(uint64_t t);

    uint64_t frequency_to_period(float frequency);

} // namespace LichensCPP