/*
 * Created on Mon Sep 22 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once
#include <string>
#include <vector>
#include "LichensCPP/config/Config.h"
#include "LichensCPP/config/ConfigValue.h"
#include "LichensCPP/config/AppConfig.h"

namespace LichensCPP
{

inline const Config& get_config(std::size_t config_type)
{
    return AppConfig::instance()[config_type];
}

template<typename T, typename = typename std::enable_if_t<
    std::is_enum<T>::value && 
    !std::is_integral<T>::value &&
    !std::is_same<T, std::size_t>::value>>
inline const Config& get_config(const T& config_type)
{
    return AppConfig::instance()[static_cast<size_t>(config_type)];
}

template<typename T,
         typename = std::enable_if_t<
             std::is_integral<T>::value &&
             !std::is_same<T, std::size_t>::value &&
             !std::is_enum<T>::value>>
inline const Config& get_config(const T& config_type)
{
    return AppConfig::instance()[static_cast<std::size_t>(config_type)];
}

inline const ConfigValue& get_config_value(std::size_t config_type, const std::string& subkey)
{
    return get_config(config_type).get_value(subkey);
}

inline const ConfigValue& get_config_value(std::size_t config_type, const std::vector<std::string>& subkeys)
{
    return get_config(config_type).get_value(subkeys);
}

template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
inline const ConfigValue& get_config_value(const T& config_type, const std::string& subkey)
{
    return get_config(config_type).get_value(subkey);
}

template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
inline const ConfigValue& get_config_value(const T& config_type, const std::vector<std::string>& subkeys)
{
    return get_config(config_type).get_value(subkeys);
}

} // namespace LichensCPP
