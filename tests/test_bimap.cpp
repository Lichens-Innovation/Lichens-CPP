/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-01-12
 */

#include <gtest/gtest.h>
#include "LichensCPP/bimap.h"
#include <string>

using namespace LichensCPP;

TEST(BiMap, ConstructAndBasics)
{
    BiMap<int, std::string> bm;
    EXPECT_EQ(bm.size(), 0u);
}

TEST(BiMap, InitializerListConstruction)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    EXPECT_EQ(bm.size(), 3u);
    EXPECT_EQ(bm.at_key(1), "one");
    EXPECT_EQ(bm.at_key(2), "two");
    EXPECT_EQ(bm.at_key(3), "three");
}

TEST(BiMap, Insert)
{
    BiMap<int, std::string> bm;
    EXPECT_TRUE(bm.insert(1, "one"));
    EXPECT_TRUE(bm.insert(2, "two"));
    EXPECT_EQ(bm.size(), 2u);
    EXPECT_EQ(bm.at_key(1), "one");
    EXPECT_EQ(bm.at_key(2), "two");
}

TEST(BiMap, InsertDuplicateKey)
{
    BiMap<int, std::string> bm;
    EXPECT_TRUE(bm.insert(1, "one"));
    EXPECT_FALSE(bm.insert(1, "uno"));
    EXPECT_EQ(bm.size(), 1u);
    EXPECT_EQ(bm.at_key(1), "one");
}

TEST(BiMap, InsertDuplicateValue)
{
    BiMap<int, std::string> bm;
    EXPECT_TRUE(bm.insert(1, "one"));
    EXPECT_FALSE(bm.insert(2, "one"));
    EXPECT_EQ(bm.size(), 1u);
}

TEST(BiMap, EraseByKey)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    EXPECT_TRUE(bm.erase_by_key(2));
    EXPECT_EQ(bm.size(), 2u);
    EXPECT_FALSE(bm.contains_key(2));
    EXPECT_FALSE(bm.contains_value("two"));
}

TEST(BiMap, EraseByKeyNotFound)
{
    BiMap<int, std::string> bm = {{1, "one"}};
    EXPECT_FALSE(bm.erase_by_key(99));
    EXPECT_EQ(bm.size(), 1u);
}

TEST(BiMap, EraseByValue)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    EXPECT_TRUE(bm.erase_by_value("two"));
    EXPECT_EQ(bm.size(), 2u);
    EXPECT_FALSE(bm.contains_key(2));
    EXPECT_FALSE(bm.contains_value("two"));
}

TEST(BiMap, EraseByValueNotFound)
{
    BiMap<int, std::string> bm = {{1, "one"}};
    EXPECT_FALSE(bm.erase_by_value("ninety-nine"));
    EXPECT_EQ(bm.size(), 1u);
}

TEST(BiMap, AtKey)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_EQ(bm.at_key(1), "one");
    EXPECT_EQ(bm.at_key(2), "two");
}

TEST(BiMap, AtKeyThrows)
{
    BiMap<int, std::string> bm = {{1, "one"}};
    EXPECT_THROW(bm.at_key(99), std::out_of_range);
}

TEST(BiMap, AtValue)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_EQ(bm.at_value("one"), 1);
    EXPECT_EQ(bm.at_value("two"), 2);
}

TEST(BiMap, AtValueThrows)
{
    BiMap<int, std::string> bm = {{1, "one"}};
    EXPECT_THROW(bm.at_value("ninety-nine"), std::out_of_range);
}

TEST(BiMap, ContainsKey)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_TRUE(bm.contains_key(1));
    EXPECT_TRUE(bm.contains_key(2));
    EXPECT_FALSE(bm.contains_key(3));
}

TEST(BiMap, ContainsValue)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_TRUE(bm.contains_value("one"));
    EXPECT_TRUE(bm.contains_value("two"));
    EXPECT_FALSE(bm.contains_value("three"));
}

TEST(BiMap, SizeOperator)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    size_t s = bm;
    EXPECT_EQ(s, 2u);
}

TEST(BiMap, Reserve)
{
    BiMap<int, std::string> bm;
    bm.reserve(100);
    EXPECT_EQ(bm.size(), 0u);
}

TEST(BiMap, SubscriptOperatorByKey)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_EQ(bm[1], "one");
    EXPECT_EQ(bm[2], "two");
}

TEST(BiMap, SubscriptOperatorByValue)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}};
    EXPECT_EQ(bm["one"], 1);
    EXPECT_EQ(bm["two"], 2);
}

TEST(BiMap, SubscriptOperatorThrows)
{
    BiMap<int, std::string> bm = {{1, "one"}};
    EXPECT_THROW(bm[99], std::out_of_range);
    EXPECT_THROW(bm["ninety-nine"], std::out_of_range);
}

TEST(BiMap, Iterator)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    int count = 0;
    for (auto it = bm.begin(); it != bm.end(); ++it)
    {
        count++;
        EXPECT_TRUE(bm.contains_key(it->first));
        EXPECT_EQ(bm.at_key(it->first), it->second);
    }
    EXPECT_EQ(count, 3);
}

TEST(BiMap, ConstIterator)
{
    const BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    int count = 0;
    for (auto it = bm.cbegin(); it != bm.cend(); ++it)
    {
        count++;
        EXPECT_TRUE(bm.contains_key(it->first));
    }
    EXPECT_EQ(count, 3);
}

TEST(BiMap, RangeBasedFor)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    int count = 0;
    for (const auto &pair : bm)
    {
        count++;
        EXPECT_EQ(bm.at_key(pair.first), pair.second);
    }
    EXPECT_EQ(count, 3);
}

TEST(BiMap, CopyConstructor)
{
    BiMap<int, std::string> bm1 = {{1, "one"}, {2, "two"}};
    BiMap<int, std::string> bm2(bm1);
    EXPECT_EQ(bm2.size(), 2u);
    EXPECT_EQ(bm2.at_key(1), "one");
    EXPECT_EQ(bm2.at_key(2), "two");
}

TEST(BiMap, MoveConstructor)
{
    BiMap<int, std::string> bm1 = {{1, "one"}, {2, "two"}};
    BiMap<int, std::string> bm2(std::move(bm1));
    EXPECT_EQ(bm2.size(), 2u);
    EXPECT_EQ(bm2.at_key(1), "one");
    EXPECT_EQ(bm2.at_key(2), "two");
}

TEST(BiMap, CopyAssignment)
{
    BiMap<int, std::string> bm1 = {{1, "one"}, {2, "two"}};
    BiMap<int, std::string> bm2;
    bm2 = bm1;
    EXPECT_EQ(bm2.size(), 2u);
    EXPECT_EQ(bm2.at_key(1), "one");
    EXPECT_EQ(bm2.at_key(2), "two");
}

TEST(BiMap, MoveAssignment)
{
    BiMap<int, std::string> bm1 = {{1, "one"}, {2, "two"}};
    BiMap<int, std::string> bm2;
    bm2 = std::move(bm1);
    EXPECT_EQ(bm2.size(), 2u);
    EXPECT_EQ(bm2.at_key(1), "one");
    EXPECT_EQ(bm2.at_key(2), "two");
}

TEST(BiMap, BidirectionalConsistency)
{
    BiMap<int, std::string> bm = {{1, "one"}, {2, "two"}, {3, "three"}};
    
    EXPECT_EQ(bm.at_value(bm.at_key(1)), 1);
    EXPECT_EQ(bm.at_value(bm.at_key(2)), 2);
    EXPECT_EQ(bm.at_value(bm.at_key(3)), 3);
    
    EXPECT_EQ(bm.at_key(bm.at_value("one")), "one");
    EXPECT_EQ(bm.at_key(bm.at_value("two")), "two");
    EXPECT_EQ(bm.at_key(bm.at_value("three")), "three");
}

TEST(BiMap, DifferentTypes)
{
    BiMap<std::string, int> bm = {{"alpha", 1}, {"beta", 2}, {"gamma", 3}};
    EXPECT_EQ(bm["alpha"], 1);
    EXPECT_EQ(bm[2], "beta");
    EXPECT_EQ(bm.at_key("gamma"), 3);
    EXPECT_EQ(bm.at_value(1), "alpha");
}