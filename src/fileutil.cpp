/*
 * Created on Fri Oct 31 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include <iostream>
#include <fstream>

#include "fileutil.h"
#include "LichensCPP/log/LoggerHelper.h"

namespace LichensCPP
{
    bool write_bytes_to_file(const std::string& path, const std::vector<std::byte>& data)
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file)
        {
            LOG_ERROR_S("Failed to open file: ", path);
            return false;
        }
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        if (!file)
        {
            LOG_ERROR_S("Failed to write to file: ", path);
            return false;
        }
        return true;
    }

    bool write_bytes_to_file(const std::string& path, const void* data, size_t size)
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file)
        {
            LOG_ERROR_S("Failed to open file: ", path);
            return false;
        }
        file.write(reinterpret_cast<const char *>(data), size);
        if (!file)
        {
            LOG_ERROR_S("Failed to write to file: ", path);
            return false;
        }
        return true;
    }

    std::optional<std::vector<std::byte>> read_bytes_from_file(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
        {
            LOG_ERROR_S("Failed to open file: ", path);
            return std::nullopt;
        }

        const auto file_size = file.tellg();
        file.seekg(0);

        std::vector<std::byte> buffer(file_size);
        file.read(reinterpret_cast<char *>(buffer.data()), file_size);
        if (!file)
        {
            LOG_ERROR_S("Failed to read from file: ", path);
            return std::nullopt;
        }

        return buffer;
    }

    bool read_bytes_from_file(const std::string& path, std::vector<std::byte>& buffer)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
        {
            LOG_ERROR_S("Failed to open file: ", path);
            return false;
        }

        const auto file_size = file.tellg();
        file.seekg(0);

        buffer.resize(file_size);
        file.read(reinterpret_cast<char*>(buffer.data()), file_size);
        if (!file)
        {
            LOG_ERROR_S("Failed to read from file: ", path);
            return false;
        }
        return true;
    }

} // namespace LichensCPP
