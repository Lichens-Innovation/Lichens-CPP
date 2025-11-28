/*
 * Created on Mon Sep 22 2025
 *
 * Copyright (c) 2025 PointLAZ Inc. All rights reserved.
 */

#include <gtest/gtest.h>
#include <sstream>

#include "LichensCPP/config/ConfigValue.h"

using namespace LichensCPP;

const ConfigValueArray empty_array;
const ConfigValueObject empty_object;

TEST(ConfigValueTest, DefaultConstructor) 
{
    ConfigValue config_value;

    EXPECT_TRUE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_FALSE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_FALSE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_TRUE(config_value.is_leaf());

    EXPECT_FALSE(config_value.get_bool());
    EXPECT_TRUE(config_value.get_bool(true));
    EXPECT_EQ(config_value.get_int(), 0);
    EXPECT_EQ(config_value.get_int(10), 10);
    EXPECT_EQ(config_value.get_double(), 0.0);
    EXPECT_EQ(config_value.get_double(-2.0), -2.0);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_string("default"), std::string("default"));
    EXPECT_EQ(config_value.get_array(), empty_array);
    EXPECT_EQ(config_value.get_object(), empty_object);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, BoolValue) 
{
    ConfigValue config_value;
    config_value.value = true;

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_TRUE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_FALSE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_FALSE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_TRUE(config_value.is_leaf());

    EXPECT_TRUE(config_value.get_bool());
    EXPECT_EQ(config_value.get_int(), 1);
    EXPECT_EQ(config_value.get_double(), 1.0);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_array(), empty_array);
    EXPECT_EQ(config_value.get_object(), empty_object);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, IntValue) 
{
    ConfigValue config_value;
    config_value.value = 42;

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_TRUE(config_value.is_int());
    EXPECT_FALSE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_TRUE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_TRUE(config_value.is_leaf());

    EXPECT_EQ(config_value.get_bool(), true);
    EXPECT_EQ(config_value.get_int(), 42);
    EXPECT_EQ(config_value.get_double(), 42.0);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_array(), empty_array);
    EXPECT_EQ(config_value.get_object(), empty_object);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, DoubleValue)
{
    ConfigValue config_value;
    config_value.value = 3.14;

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_TRUE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_TRUE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_TRUE(config_value.is_leaf());

    EXPECT_EQ(config_value.get_bool(), true);
    EXPECT_EQ(config_value.get_int(), 3);
    EXPECT_EQ(config_value.get_double(), 3.14);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_array(), empty_array);
    EXPECT_EQ(config_value.get_object(), empty_object);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, StringValue)
{
    ConfigValue config_value;
    config_value.value = "Hello, World!";

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_TRUE(config_value.is_string());
    EXPECT_FALSE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_TRUE(config_value.is_leaf());

    EXPECT_EQ(config_value.get_bool(), false);
    EXPECT_EQ(config_value.get_int(), 0);
    EXPECT_EQ(config_value.get_double(), 0.0);
    EXPECT_EQ(config_value.get_string(), std::string("Hello, World!"));
    EXPECT_EQ(config_value.get_array(), empty_array);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(1).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, ArrayValue)
{
    ConfigValue config_value;
    config_value.value = ConfigValueArray{
        make_container_value(1), 
        make_container_value(2), 
        make_container_value(3)
    };

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_FALSE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_FALSE(config_value.is_numerical());
    EXPECT_TRUE(config_value.is_array());
    EXPECT_FALSE(config_value.is_object());
    EXPECT_FALSE(config_value.is_leaf());

    EXPECT_EQ(config_value.get_bool(), false);
    EXPECT_EQ(config_value.get_int(), 0);
    EXPECT_EQ(config_value.get_double(), 0.0);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_array().size(), 3u);
    EXPECT_EQ(config_value.get_object(), empty_object);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).get_int() == 1);
    EXPECT_TRUE(config_value.get_child(1).get_int() == 2);
    EXPECT_TRUE(config_value.get_child(2).get_int() == 3);
    EXPECT_TRUE(config_value.get_child(3).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].get_int() == 1);
    EXPECT_TRUE(config_value[1].get_int() == 2);
    EXPECT_TRUE(config_value[2].get_int() == 3);
    EXPECT_TRUE(config_value[3].value == ConfigValue::NULL_CONFIG_VALUE.value);
}

TEST(ConfigValueTest, ObjectValue)
{
    ConfigValue config_value;
    config_value.value = ConfigValueObject{
        {"a", make_container_value(1)},
        {"b", make_container_value(2)},
        {"c", make_container_value(3)},
        {"d", make_container_value(ConfigValueObject{
            {"e", make_container_value(3)}
        })}
    };

    EXPECT_FALSE(config_value.is_empty());
    EXPECT_FALSE(config_value.is_bool());
    EXPECT_FALSE(config_value.is_int());
    EXPECT_FALSE(config_value.is_double());
    EXPECT_FALSE(config_value.is_string());
    EXPECT_FALSE(config_value.is_numerical());
    EXPECT_FALSE(config_value.is_array());
    EXPECT_TRUE(config_value.is_object());
    EXPECT_FALSE(config_value.is_leaf());

    EXPECT_EQ(config_value.get_bool(), false);
    EXPECT_EQ(config_value.get_int(), 0);
    EXPECT_EQ(config_value.get_double(), 0.0);
    EXPECT_EQ(config_value.get_string(), std::string());
    EXPECT_EQ(config_value.get_array(), empty_array);
    EXPECT_EQ(config_value.get_object().size(), 4u);

    EXPECT_TRUE(config_value.get_child("nonexistent").value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(ConfigMultiKey{"toto", "tata"}).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value.get_child(0).value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value["nonexistent"].value == ConfigValue::NULL_CONFIG_VALUE.value);
    EXPECT_TRUE(config_value[0].value == ConfigValue::NULL_CONFIG_VALUE.value);

    EXPECT_EQ(config_value.get_child("a").get_int(), 1);
    EXPECT_EQ(config_value.get_child("b").get_int(), 2);
    EXPECT_EQ(config_value.get_child("c").get_int(), 3);

    EXPECT_EQ(config_value["a"].get_int(), 1);
    EXPECT_EQ(config_value["b"].get_int(), 2);
    EXPECT_EQ(config_value["c"].get_int(), 3);

    EXPECT_EQ(config_value.get_child("d").get_object().size(), 1u);
    EXPECT_EQ(config_value.get_child("d").get_child("e").get_int(), 3);
    EXPECT_EQ(config_value["d"]["e"].get_int(), 3);
    EXPECT_EQ(config_value.get_child(ConfigMultiKey{"d", "e"}).get_int(), 3);
}

TEST(ConfigValueTest, OstreamOutput)
{
    ConfigValue config_value;
    config_value.value = ConfigValueObject{
        {"a", make_container_value(1)},
        {"b", make_container_value(ConfigValueObject{
            {"c", make_container_value(2)}
        })},
        {"d", make_container_value(ConfigValueArray{
            make_container_value(3), 
            make_container_value(4)
        })}
    };

    std::stringstream ss;
    ss << config_value;
    // NOTE: this is fragile since it can change based on unordered_map order
    EXPECT_EQ(ss.str(), "{\"b\": {\"c\": 2}, \"d\": [3, 4], \"a\": 1}");
}

TEST(ConfigValueTest, ObjectMerge)
{
    ConfigValueObject config_value_1 {
        {"a", make_container_value(1)},
        {"b", make_container_value(2)},
        {"d", make_container_value(ConfigValueObject{
            {"e", make_container_value(10)},
            {"f", make_container_value(11)}
        })}
    };

    ConfigValueObject config_value_2 {
        {"b", make_container_value(3)},
        {"c", make_container_value(4)},
        {"d", make_container_value(ConfigValueObject{
            {"f", make_container_value(12)},
            {"g", make_container_value(13)}
        })}
    };

    ConfigValue config_merged = merge_config_objects(config_value_1, config_value_2);

    EXPECT_EQ(config_merged["a"].get_int(), 1);
    EXPECT_EQ(config_merged["b"].get_int(), 3);
    EXPECT_EQ(config_merged["c"].get_int(), 4);
    EXPECT_EQ(config_merged["d"]["e"].get_int(), 10);
    EXPECT_EQ(config_merged["d"]["f"].get_int(), 12);
    EXPECT_EQ(config_merged["d"]["g"].get_int(), 13);
}