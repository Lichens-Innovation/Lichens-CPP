/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Tue Oct 07 2025
 */

#include <gtest/gtest.h>
#include "LichensCPP/stringutil.h"

using namespace LichensCPP;

TEST(StringUtilTest, ToLower) 
{
    std::string str = "Hello World";
    EXPECT_EQ(to_lower(str), "hello world");
    EXPECT_EQ(to_lower(""), "");
}

TEST(StringUtilTest, ToUpper) 
{
    std::string str = "Hello World";
    EXPECT_EQ(to_upper(str), "HELLO WORLD");
    EXPECT_EQ(to_upper(""), "");
}

TEST(StringUtilTest, ReplaceFirst)
{
    std::string str = "foo bar foo";
    EXPECT_EQ(replace_first(str, "foo", "baz"), "baz bar foo");
    EXPECT_EQ(replace_first(str, "qux", "baz"), "foo bar foo"); // no match
    EXPECT_EQ(replace_first("", "foo", "baz"), "");
}

TEST(StringUtilTest, ReplaceAll)
{
    std::string str = "foo bar foo";
    EXPECT_EQ(replace_all(str, "foo", "baz"), "baz bar baz");
    EXPECT_EQ(replace_all(str, "qux", "baz"), "foo bar foo"); // no match
    EXPECT_EQ(replace_all("", "foo", "baz"), "");
}

TEST(StringUtilTest, Base64EncodeDecodeEmpty)
{
    // Test empty input
    EXPECT_EQ(base64_encode({}), "");
    EXPECT_EQ(base64_decode("").size(), 0u);
}

TEST(StringUtilTest, Base64EncodeDecode)
{
    // Test known values
    std::vector<std::byte> data = {std::byte('h'), std::byte('i')};
    const std::string encoded = base64_encode(data);
    EXPECT_EQ(encoded, "aGk=");
    const std::vector<std::byte> decoded = base64_decode(encoded);
    ASSERT_EQ(decoded.size(), data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(decoded[i], data[i]);
    }
}