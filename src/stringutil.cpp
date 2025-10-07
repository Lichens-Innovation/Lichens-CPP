/*
 * Created on Fri Oct 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/stringutil.h"
#include <algorithm>
#include <cctype>

namespace LichensCPP
{
    std::string& to_lower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return str;
    }

    std::string to_lower(const std::string& str)
    {
        std::string lower_str;
        lower_str.resize(str.size());
        std::transform(str.begin(), str.end(), lower_str.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return lower_str;
    }

    std::string& to_upper(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return str;
    }

    std::string to_upper(const std::string& str)
    {
        std::string upper_str;
        upper_str.resize(str.size());
        std::transform(str.begin(), str.end(), upper_str.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return upper_str;
    }
}