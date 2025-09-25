/*
 * Created on Fri Sep 19 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <ostream>

namespace LichensCPP
{

struct ConfigValue;

using ContainerValue = std::shared_ptr<ConfigValue>;
using ConfigValueArray = std::vector<ContainerValue>;
using ConfigValueObject = std::unordered_map<std::string, ContainerValue>;
using ConfigMultiKey = std::vector<std::string>;

template <typename... Args>
inline std::shared_ptr<ConfigValue> make_container_value(Args&&... args) {
    return std::make_shared<ConfigValue>(std::forward<Args>(args)...);
}

using ConfigValueType = std::variant<
    std::nullptr_t, // Empty no value or nullptr
    bool,
    int,
    double,
    std::string,
    ConfigValueArray,
    ConfigValueObject
>;

struct ConfigValue
{
    ConfigValueType value;

    static const ConfigValue NULL_CONFIG_VALUE;

    ConfigValue();
    ConfigValue(bool v);
    ConfigValue(int v);
    ConfigValue(double v);
    ConfigValue(const std::string& v);
    ConfigValue(const ConfigValueArray& v);
    ConfigValue(const ConfigValueObject& v);

    bool is_empty() const;
    bool is_bool() const;
    bool is_int() const;
    bool is_double() const;
    bool is_string() const;
    
    bool is_leaf() const;

    bool is_numerical() const;
    bool is_array() const;
    bool is_object() const;

    bool get_bool(bool default_value = false) const;
    int get_int(int default_value = 0) const;
    double get_double(double default_value = 0.0) const;
    std::string get_string(const std::string& default_value = "") const;
    ConfigValueArray get_array() const;
    ConfigValueObject get_object() const;

    const ConfigValue& get_child(const std::string& child) const;
    const ConfigValue& get_child(const ConfigMultiKey& child) const;
    const ConfigValue& get_child(size_t index) const;
    const ConfigValue& operator[](const std::string& child) const;
    const ConfigValue& operator[](size_t index) const;
};

ConfigValueObject merge_config_objects(const ConfigValueObject& a, const ConfigValueObject& b);

std::ostream& operator<<(std::ostream& os, const ConfigValue& cv);

} // namespace LichensCPP