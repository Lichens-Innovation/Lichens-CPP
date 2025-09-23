/*
 * Created on Fri Sep 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/config/Config.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "LichensCPP/log/LoggerHelper.h"

namespace LichensCPP
{

struct Config::ConfigPrivate
{
    ConfigValueObject root;

    ConfigValue toValue(const nlohmann::json& j)
    {
        ConfigValue v;

        if (j.is_object()) 
        {
            ConfigValueObject obj;
            for (const auto& [k, val] : j.items())
            {
                auto child_val = toValue(val);
                obj.insert(std::make_pair(k, std::make_shared<ConfigValue>(child_val)));
            }
            v.value = obj;
        } 
        else if (j.is_array())
        {
            ConfigValueArray arr;
            for (const auto& e : j)
            {
                arr.push_back(std::make_shared<ConfigValue>(toValue(e)));
            }
            v.value = arr;
        } 
        else if (j.is_string()) 
        {
            v.value = j.get<std::string>();
        }
        else if (j.is_boolean()) 
        {
            v.value = j.get<bool>();
        }
        else if (j.is_number_integer()) 
        {
            v.value = j.get<int>();
        }
        else if (j.is_number_float()) 
        {
            v.value = j.get<double>();
        }
        else 
        {
            v.value = nullptr;
        }

        return v;
    }

    bool load_config_json(const nlohmann::json& j)
    {
        try
        {
            auto conf_value = toValue(j);
            if(!conf_value.is_object())
            {
                LOG_ERROR_S("Top level of JSON config must be an object");
                return false;
            }
            root = merge_config_objects(root, conf_value.get_object());
        }
        catch (const std::exception& e)
        {
            LOG_ERROR_S("Failed to parse JSON config.");
            return false;
        }
        return true;
    }

    bool load_config_json_string(const std::string& json_data)
    {
        try
        {
            auto j = nlohmann::json::parse(json_data);
            load_config_json(j);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR_S("Failed to parse JSON config string: ", e.what());
            return false;
        }
        return true;
    }

    bool load_config_json_file(const std::string& file_path)
    {
        try
        {
            std::ifstream ifs(file_path);
            if (!ifs.is_open())
            {
                LOG_ERROR_S("Failed to open config file: ", file_path);
                return false;
            }
            nlohmann::json j;
            ifs >> j;
            load_config_json(j);
            ifs.close();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR_S("Failed to parse JSON config file ", file_path, ": ", e.what());
            return false;
        }
        return true;
    }
};

Config::Config()
    : p_(new ConfigPrivate())
{
}

Config::Config(const Config& other)
    : p_(new ConfigPrivate())
{
    *this = other;
}

Config::Config(Config&& other) noexcept
    : p_(std::move(other.p_))
{
    other.p_ = nullptr;
}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        // Deep copy the root object
        p_->root = other.p_->root;
    }
    return *this;
}

Config::~Config()
{
    clear();
}

void Config::clear()
{
    p_->root.clear();
}

bool Config::load_config(const std::string& file_path)
{
    try
    {
        if(file_path.empty())
        {
            LOG_ERROR("Config file path is empty");
            return false;
        }
        if(!std::filesystem::exists(file_path))
        {
            LOG_ERROR_S("Config file does not exist: ", file_path);
            return false;
        }

        // Check extension
        // Make better type detection later not only based on extension
        std::string ext = std::filesystem::path(file_path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](char c) { return std::tolower(c); });
        
        if(ext != ".json")
        {
            p_->load_config_json_file(file_path);
        }
        // TODO add support for yaml, toml, ini, xml, etc.
        else
        {
            LOG_ERROR_S("Unsupported config file format: ", ext);
            return false;
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR_S("Failed to load config file ", file_path, ": ", e.what());
        return false;
    }
    return true;
}

bool Config::load_config_json(const std::string& json_data)
{
    return p_->load_config_json_string(json_data);
}

const ConfigValue& Config::get_value(const std::string& key) const
{
    if (!key.empty() && p_->root.find(key) != p_->root.end())
    {
        return *(p_->root.at(key));
    }
    return ConfigValue::NULL_CONFIG_VALUE;
}

const ConfigValue& Config::get_value(const ConfigMultiKey& keys) const
{
    if(keys.empty())
    {
        return ConfigValue::NULL_CONFIG_VALUE;
    }
    const auto* current = &get_value(keys[0]);
    if(keys.size() == 1)
    {
        return *current;
    }

    for(auto itr = keys.begin() + 1; itr != keys.end(); ++itr)
    {
        if (current != nullptr || itr->empty())
        {
            return ConfigValue::NULL_CONFIG_VALUE;
        }
        current = &current->get_child(*itr);
    }
    return *current;
}

const ConfigValue& Config::operator[](const std::string& key) const
{
    return get_value(key);
}

} // namespace LichensCPP
