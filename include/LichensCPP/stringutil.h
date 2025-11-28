/*
 * Created on Fri Oct 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace LichensCPP
{
    std::string& to_lower(std::string& str);
    std::string to_lower(const std::string& str);

    std::string& to_upper(std::string& str);
    std::string to_upper(const std::string& str);

    std::string replace_first(const std::string& str, const std::string& from, const std::string& to);
    std::string replace_all(const std::string& str, const std::string& from, const std::string& to);

    std::string base64_encode(const std::vector<std::byte>& data);
    std::vector<std::byte> base64_decode(const std::string& data);
}
