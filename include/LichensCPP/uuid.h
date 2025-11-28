/*
 * Created on Wed Nov 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <string>

namespace LichensCPP
{
    std::string generate_uuid_v4();
    bool is_valid_uuid_v4(const std::string& uuid);
}