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
}

TEST(StringUtilTest, ToUpper) 
{
    std::string str = "Hello World";
    EXPECT_EQ(to_upper(str), "HELLO WORLD");
}