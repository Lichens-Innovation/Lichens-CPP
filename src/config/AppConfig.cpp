/*
 * Created on Fri Sep 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/config/AppConfig.h"

namespace LichensCPP
{

struct AppConfig::AppConfigPrivate
{
    std::unordered_map<size_t, Config> configs;
};

AppConfig::AppConfig()
    : p_(new AppConfigPrivate())
{
}

AppConfig::~AppConfig()
{
    p_.reset();
}

AppConfig& AppConfig::instance()
{
    static AppConfig _global_instance;
    return _global_instance;
}

void AppConfig::add_config(const size_t& key, const Config& config)
{
    p_->configs.insert({key, config});
}

void AppConfig::add_config(const size_t& key, Config&& config)
{
    p_->configs.insert({key, std::move(config)});
}

const Config& AppConfig::get_config(const size_t& key) const
{
    auto it = p_->configs.find(key);
    if (it != p_->configs.end())
    {
        return it->second;
    }
    throw std::runtime_error("Config not found");
}

const Config& AppConfig::operator[](const size_t& key) const
{
    return get_config(key);
}

} // namespace LichensCPP