/*
 * Created on Mon Sep 15 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "LichensCPP/config/ConfigValue.h"

namespace LichensCPP
{

class Config
{
public:
    explicit Config();
    ~Config();

    Config(const Config& other);
    Config(Config&& other) noexcept;
    Config& operator=(const Config& other);

    // Clear all loaded configuration
    void clear();

    /// @brief Load configuration from a file. This is cumulative and merges with existing configuration.
    ///     If you need to replace the configuration, call clear() first.
    /// @param file_path The path to the configuration file.
    /// @return True if the configuration was loaded successfully, false otherwise.
    bool load_config(const std::string& file_path);
    bool load_config_json(const std::string& json_data);

    /// @brief Get a reference to a configuration value.
    /// @param key The key of the configuration value.
    /// @return A reference to the configuration value, or std::nullopt if not found.
    const ConfigValue& get_value(const std::string& key) const;
    const ConfigValue& get_value(const ConfigMultiKey& keys) const;

    const ConfigValue& operator[](const std::string& key) const;

private:
    struct ConfigPrivate;
    std::unique_ptr<ConfigPrivate> p_;
};

} // namespace LichensCPP