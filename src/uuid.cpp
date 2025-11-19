/*
 * Created on Wed Nov 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/uuid.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace LichensCPP
{
    std::string generate_uuid_v4()
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());

        // UUID layout: 8-4-4-4-12 hex chars = 36 chars
        std::uniform_int_distribution<uint64_t> dis;

        uint64_t part1 = dis(gen);
        uint64_t part2 = dis(gen);

        // Convert to string
        std::stringstream ss;

        // 8 hex (32 bits)
        ss << std::hex << std::setfill('0') << std::setw(8) << (uint32_t)(part1 >> 32);

        ss << "-";

        // 4 hex (16 bits)
        ss << std::setw(4) << (uint16_t)(part1 >> 16);

        ss << "-";

        // 4 hex with version 4 (UUID v4)
        ss << std::setw(4) << ((uint16_t)((part1 & 0xFFFF) & 0x0FFF) | 0x4000);

        ss << "-";

        // 4 hex with variant 1 (0b10xxxxxx)
        ss << std::setw(4) << ((uint16_t)(((part2 >> 48) & 0x3FFF) | 0x8000));

        ss << "-";

        // 12 hex (48 bits)
        ss << std::setw(12) << (uint64_t)(part2 & 0x0000FFFFFFFFFFFFULL);

        return ss.str();
    }

    bool is_valid_uuid_v4(const std::string& uuid)
    {
        if (uuid.length() != 36)
        {
            return false;
        }

        for (size_t i = 0; i < uuid.length(); ++i)
        {
            char c = uuid[i];
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                if (c != '-')
                    return false;
            }
            else
            {
                if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
                    return false;
            }
        }

        // Check version (should be '4')
        if (uuid[14] != '4')
        {
            return false;
        }

        // Check variant (should be one of '8', '9', 'a', or 'b')
        char variant = uuid[19];
        return (variant == '8' 
            || variant == '9' 
            || variant == 'a' 
            || variant == 'b' 
            || variant == 'A' 
            || variant == 'B');
    }

} // namespace LichensCPP