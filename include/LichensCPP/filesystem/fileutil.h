/*
 * Created on Fri Oct 31 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace LichensCPP
{
    bool write_bytes_to_file(const std::string& path, const std::vector<std::byte>& data);
    bool write_bytes_to_file(const std::string& path, const void* data, size_t size);

    std::optional<std::vector<std::byte>> read_bytes_from_file(const std::string& path);
    bool read_bytes_from_file(const std::string &path, std::vector<std::byte>& buffer);

} // namespace LichensCPP
