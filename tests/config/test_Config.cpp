/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Tue Sep 23 2025
 */

#include <gtest/gtest.h>

#include "LichensCPP/config/Config.h"

using namespace LichensCPP;

TEST(ConfigTest, DefaultConstructor) 
{
    Config config;

    EXPECT_EQ(config.get_value("key1").value, ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigTest, JsonConfig)
{
    const std::string json_string = R"({
        "key1": "value1",
        "key2": 42,
        "key3": 3.14,
        "key4": true,
        "key5": null,
        "key6": [1, 2, 3],
        "key7": {"nested_key": "nested_value"}
    })";

    Config config; 
    config.load_config_json(json_string);

    EXPECT_TRUE(config.get_value("key5").is_empty());
    EXPECT_TRUE(config.get_value("key1").is_string());
    EXPECT_TRUE(config.get_value("key2").is_int());
    EXPECT_TRUE(config.get_value("key3").is_double());
    EXPECT_TRUE(config.get_value("key4").is_bool());
    EXPECT_TRUE(config.get_value("key6").is_array());
    EXPECT_TRUE(config.get_value("key7").is_object());

    EXPECT_EQ(config.get_value("key1").get_string(), "value1");
    EXPECT_EQ(config.get_value("key2").get_int(), 42);
    EXPECT_EQ(config.get_value("key3").get_double(), 3.14);
    EXPECT_EQ(config.get_value("key4").get_bool(), true);
    EXPECT_EQ(config.get_value("key6").get_array().size(), 3u);
    EXPECT_EQ(config.get_value("key7").get_object().size(), 1u);

    EXPECT_EQ(config["key6"][0].get_int(), 1);
    EXPECT_EQ(config["key6"][1].get_int(), 2);
    EXPECT_EQ(config["key6"][2].get_int(), 3);
    EXPECT_EQ(config["key7"]["nested_key"].get_string(), "nested_value");
}

TEST(ConfigTest, ConfigMerge)
{
    const std::string json_string1 = R"({
        "key1": 42,
        "key2": 43
    })";

    const std::string json_string2 = R"({
        "key2": 123,
        "key3": 124
    })";

    Config config; 
    config.load_config_json(json_string1);
    config.load_config_json(json_string2);

    EXPECT_TRUE(config.get_value("key1").is_int());
    EXPECT_TRUE(config.get_value("key2").is_int());
    EXPECT_TRUE(config.get_value("key3").is_int());

    EXPECT_EQ(config.get_value("key1").get_int(), 42);
    EXPECT_EQ(config.get_value("key2").get_int(), 123);
    EXPECT_EQ(config.get_value("key3").get_int(), 124);
}

TEST(ConfigTest, ConfigClear)
{
    const std::string json_string = R"({
        "key1": 42
    })";
    Config config; 
    config.load_config_json(json_string);

    EXPECT_TRUE(config.get_value("key1").is_int());
    EXPECT_EQ(config.get_value("key1").get_int(), 42);

    config.clear();

    EXPECT_TRUE(config.get_value("key1").is_empty());
    EXPECT_FALSE(config.get_value("key1").is_int());
}