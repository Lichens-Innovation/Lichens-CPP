/*
 * Created on Fri Oct 03 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/stringutil.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <stdexcept>

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

    std::string replace_first(const std::string &str, const std::string &from, const std::string &to)
    {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
        {
            return str; // Nothing to replace
        }
        std::string result = str;
        result.replace(start_pos, from.length(), to);
        return result;
    }

    std::string replace_all(const std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty())
        {
            return str; // Nothing to replace
        }

        std::string result = str;
        size_t start_pos = 0;
        while ((start_pos = result.find(from, start_pos)) != std::string::npos)
        {
            result.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Move past the replacement
        }
        return result;
    }

    static const char table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::vector<std::byte>& data)
    {
        const uint8_t *bytes = reinterpret_cast<const uint8_t*>(data.data());
        size_t len = data.size();
        std::string out;
        out.reserve(((len + 2u) / 3u) * 4u);

        for (size_t i = 0u; i < len; i += 3u)
        {
            uint32_t chunk = bytes[i] << 16;
            if (i + 1 < len)
                chunk |= bytes[i + 1] << 8;
            if (i + 2 < len)
                chunk |= bytes[i + 2];

            out.push_back(table[(chunk >> 18) & 0x3F]);
            out.push_back(table[(chunk >> 12) & 0x3F]);
            out.push_back((i + 1 < len) ? table[(chunk >> 6) & 0x3F] : '=');
            out.push_back((i + 2 < len) ? table[chunk & 0x3F] : '=');
        }
        return out;
    }

    std::vector<std::byte> base64_decode(const std::string &data)
    {
        static const int decode_table[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0, -1, -1,
            -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};

        size_t len = data.size();
        if (len % 4u != 0u)
            throw std::runtime_error("Invalid base64 data length");

        size_t out_len = len / 4u * 3u;
        if (data[len - 1] == '=')
            out_len--;
        if (data[len - 2] == '=')
            out_len--;

        std::vector<std::byte> out(out_len);
        uint8_t* out_bytes = reinterpret_cast<uint8_t*>(out.data());

        size_t j = 0u;
        for (size_t i = 0u; i < len; i += 4u)
        {
            uint32_t val = static_cast<uint32_t>(
                (decode_table[static_cast<unsigned char>(data[i])] << 18) |
                (decode_table[static_cast<unsigned char>(data[i + 1])] << 12) |
                ((data[i + 2] == '=') ? 0 : (decode_table[static_cast<unsigned char>(data[i + 2])] << 6)) |
                ((data[i + 3] == '=') ? 0 : decode_table[static_cast<unsigned char>(data[i + 3])]));

            out_bytes[j++] = (val >> 16) & 0xFF;
            if (data[i + 2] != '=')
            {
                out_bytes[j++] = (val >> 8) & 0xFF;
            }
            if (data[i + 3] != '=')
            {
                out_bytes[j++] = val & 0xFF;
            }
        }

        return out;
    }
}
