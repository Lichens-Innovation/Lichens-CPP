/*
 * Created on Fri Sep 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/config/ConfigValue.h"

namespace LichensCPP
{

    const ConfigValue ConfigValue::NULL_CONFIG_VALUE = {};

    ConfigValue::ConfigValue()
        : value(nullptr)
    {
    }

    ConfigValue::ConfigValue(bool v)
        : value(v)
    {
    }

    ConfigValue::ConfigValue(int v)
        : value(v)
    {
    }

    ConfigValue::ConfigValue(double v)
        : value(v)
    {
    }

    ConfigValue::ConfigValue(const std::string &v)
        : value(v)
    {
    }

    ConfigValue::ConfigValue(const ConfigValueArray &v)
        : value(v)
    {
    }

    ConfigValue::ConfigValue(const ConfigValueObject &v)
        : value(v)
    {
    }

    bool ConfigValue::is_empty() const
    {
        return std::holds_alternative<std::nullptr_t>(value);
    }

    bool ConfigValue::is_bool() const
    {
        return std::holds_alternative<bool>(value);
    }

    bool ConfigValue::is_int() const
    {
        return std::holds_alternative<int>(value);
    }

    bool ConfigValue::is_double() const
    {
        return std::holds_alternative<double>(value);
    }

    bool ConfigValue::is_string() const
    {
        return std::holds_alternative<std::string>(value);
    }

    bool ConfigValue::is_leaf() const
    {
        return is_empty() || is_bool() || is_int() || is_double() || is_string();
    }

    bool ConfigValue::is_numerical() const
    {
        return is_int() || is_double();
    }

    bool ConfigValue::is_array() const
    {
        return std::holds_alternative<ConfigValueArray>(value);
    }

    bool ConfigValue::is_object() const
    {
        return std::holds_alternative<ConfigValueObject>(value);
    }

    bool ConfigValue::get_bool(bool default_value) const
    {
        if (is_bool())
        {
            return std::get<bool>(value);
        }
        else if (is_int())
        {
            return std::get<int>(value) != 0;
        }
        else if (is_double())
        {
            return std::get<double>(value) != 0.0;
        }
        return default_value;
    }

    int ConfigValue::get_int(int default_value) const
    {
        if (is_int())
        {
            return std::get<int>(value);
        }
        else if (is_double())
        {
            return static_cast<int>(std::get<double>(value));
        }
        else if (is_bool())
        {
            return std::get<bool>(value) ? 1 : 0;
        }
        return default_value;
    }
    double ConfigValue::get_double(double default_value) const
    {
        if (is_double())
        {
            return std::get<double>(value);
        }
        else if (is_int())
        {
            return static_cast<double>(std::get<int>(value));
        }
        else if (is_bool())
        {
            return std::get<bool>(value) ? 1.0 : 0.0;
        }

        return default_value;
    }

    std::string ConfigValue::get_string(const std::string &default_value) const
    {
        if (is_string())
        {
            return std::get<std::string>(value);
        }
        return default_value;
    }

    ConfigValueArray ConfigValue::get_array() const
    {
        if (is_array())
        {
            return std::get<ConfigValueArray>(value);
        }
        return {};
    }

    ConfigValueObject ConfigValue::get_object() const
    {
        if (is_object())
        {
            return std::get<ConfigValueObject>(value);
        }
        return {};
    }

    const ConfigValue &ConfigValue::get_child(const std::string &child) const
    {
        if (is_object())
        {
            const auto &obj = get_object();
            auto it = obj.find(child);
            if (it != obj.end())
            {
                return *(it->second);
            }
        }
        return NULL_CONFIG_VALUE;
    }

    const ConfigValue &ConfigValue::get_child(const ConfigMultiKey &child) const
    {
        auto *current = this;
        for (const auto &key : child)
        {
            auto &next = current->get_child(key);
            if (next.is_empty())
            {
                return NULL_CONFIG_VALUE;
            }
            current = &next;
        }
        return *current;
    }

    const ConfigValue &ConfigValue::get_child(size_t index) const
    {
        if (is_array())
        {
            const auto &arr = get_array();
            if (index < arr.size())
            {
                return *(arr[index]);
            }
        }
        return NULL_CONFIG_VALUE;
    }

    const ConfigValue &ConfigValue::operator[](const std::string &child) const
    {
        return get_child(child);
    }

    const ConfigValue &ConfigValue::operator[](size_t index) const
    {
        return get_child(index);
    }

    std::ostream &operator<<(std::ostream &os, const ConfigValue &cv)
    {
        if (cv.is_empty())
        {
            os << "null";
        }
        else if (cv.is_bool())
        {
            os << (cv.get_bool() ? "true" : "false");
        }
        else if (cv.is_int())
        {
            os << cv.get_int();
        }
        else if (cv.is_double())
        {
            os << cv.get_double();
        }
        else if (cv.is_string())
        {
            os << "\"" << cv.get_string() << "\"";
        }
        else if (cv.is_array())
        {
            os << cv.get_array();
        }
        else if (cv.is_object())
        {
            os << cv.get_object();
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const ConfigValueArray &arr)
    {
        os << "[";
        for (size_t i = 0; i < arr.size(); ++i)
        {
            os << *(arr[i]);
            if (i < arr.size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const ConfigValueObject &obj)
    {
        os << "{";
        size_t count = 0;
        for (const auto &[k, v] : obj)
        {
            os << "\"" << k << "\": " << *v;
            if (count < obj.size() - 1)
            {
                os << ", ";
            }
            ++count;
        }
        os << "}";
        return os;
    }

    ConfigValueObject merge_config_objects(const ConfigValueObject &a, const ConfigValueObject &b)
    {
        ConfigValueObject result = a;

        for (const auto &[key, b_val] : b)
        {
            const auto it = result.find(key);
            if (it != result.cend())
            {
                // Key exists in a
                if (it->second->is_object() && b_val->is_object())
                {
                    // Recursively merge objects
                    it->second->value = merge_config_objects(it->second->get_object(), b_val->get_object());
                }
                else
                {
                    // Overwrite with b_val no matter the mismatch type
                    // TODO check if array should be merged instead ???
                    it->second = b_val;
                }
            }
            else
            {
                // Key missing in a, just insert
                result[key] = b_val;
            }
        }

        return result;
    }

} // namespace LichensCPP
