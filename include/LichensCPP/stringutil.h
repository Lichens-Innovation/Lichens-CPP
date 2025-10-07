/*
 * Created on Fri Oct 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <string>

namespace LichensCPP
{
    std::string& to_lower(std::string& str);
    std::string to_lower(const std::string& str);

    std::string& to_upper(std::string& str);
    std::string to_upper(const std::string& str);
}