/*
 * Created on Mon Sep 15 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "LichensCPP/config/Config.h"

namespace LichensCPP
{

class AppConfig
{
public:
    static AppConfig& instance();

    void add_config(const size_t& key, const Config& config);
    void add_config(const size_t& key, Config&& config);
    const Config& get_config(const size_t& key) const;
    const Config& operator[](const size_t& key) const;

private:
    AppConfig();
    ~AppConfig();

    struct AppConfigPrivate;
    std::unique_ptr<AppConfigPrivate> p_;
};

} // namespace LichensCPP
