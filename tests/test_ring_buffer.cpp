/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2025-11-28
 */

#include <gtest/gtest.h>
#include "LichensCPP/ring_buffer.h"

using namespace LichensCPP;

TEST(RingBuffer, ConstructAndBasics)
{
    RingBuffer<int> rb(5);
    EXPECT_EQ(rb.capacity(), 5u);
    EXPECT_EQ(rb.size(), 0u);
    EXPECT_TRUE(rb.empty());
    EXPECT_FALSE(rb.full());
}

TEST(RingBuffer, PushAndAccess)
{
    RingBuffer<int> rb(3u);

    rb.push(10);
    rb.push(20);
    rb.push(30);

    EXPECT_EQ(rb.size(), 3u);
    EXPECT_TRUE(rb.full());
    EXPECT_FALSE(rb.empty());

    EXPECT_EQ(rb[0], 10);
    EXPECT_EQ(rb[1], 20);
    EXPECT_EQ(rb[2], 30);

    EXPECT_EQ(rb.front(), 10);
    EXPECT_EQ(rb.back(), 30);
}

TEST(RingBuffer, OverwriteOldestWhenFull)
{
    RingBuffer<int> rb(3);

    rb.push(10);
    rb.push(20);
    rb.push(30);

    // Buffer full; next push overwrites the oldest (10)
    rb.push(40);

    EXPECT_EQ(rb.size(), 3u);
    EXPECT_EQ(rb[0], 20);
    EXPECT_EQ(rb[1], 30);
    EXPECT_EQ(rb[2], 40);

    EXPECT_EQ(rb.front(), 20);
    EXPECT_EQ(rb.back(), 40);
}

TEST(RingBuffer, WrapAroundCorrectOrder)
{
    RingBuffer<int> rb(3);

    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);   // overwrite 1
    rb.push(5);   // overwrite 2

    std::vector<int> expected = {3, 4, 5};

    ASSERT_EQ(rb.size(), 3u);

    for (size_t i = 0; i < rb.size(); ++i)
        EXPECT_EQ(rb[i], expected[i]);
}

TEST(RingBuffer, IteratorForward)
{
    RingBuffer<int> rb(4);
    rb.push(10);
    rb.push(20);
    rb.push(30);

    std::vector<int> values;
    for (auto it = rb.begin(); it != rb.end(); ++it)
        values.push_back(*it);

    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

TEST(RingBuffer, ConstIteratorForward)
{
    RingBuffer<int> rb(4);
    rb.push(5);
    rb.push(6);
    rb.push(7);

    const RingBuffer<int>& crb = rb;

    std::vector<int> values;
    for (auto it = crb.cbegin(); it != crb.cend(); ++it)
        values.push_back(*it);

    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 5);
    EXPECT_EQ(values[1], 6);
    EXPECT_EQ(values[2], 7);
}

TEST(RingBuffer, RangeForLoop)
{
    RingBuffer<int> rb(4);
    rb.push(100);
    rb.push(200);
    rb.push(300);

    std::vector<int> result;
    for (int v : rb) { result.push_back(v); }

    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 100);
    EXPECT_EQ(result[1], 200);
    EXPECT_EQ(result[2], 300);
}

TEST(RingBuffer, IteratorAfterWrapAround)
{
    RingBuffer<int> rb(3);

    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4); // overwrite 1

    std::vector<int> result;
    for (int v : rb)
        result.push_back(v);

    std::vector<int> expected = {2, 3, 4};
    EXPECT_EQ(result, expected);
}

TEST(RingBuffer, FrontBackCorrectness)
{
    RingBuffer<int> rb(3);

    rb.push(10);
    rb.push(20);
    rb.push(30);

    EXPECT_EQ(rb.front(), 10);
    EXPECT_EQ(rb.back(), 30);

    rb.push(40); // overwrites 10

    EXPECT_EQ(rb.front(), 20);
    EXPECT_EQ(rb.back(), 40);
}

TEST(RingBuffer, OutOfRangeAccessThrows)
{
    RingBuffer<int> rb(2);
    rb.push(1);
    EXPECT_THROW(rb[1], std::out_of_range);
}

TEST(RingBuffer, EmptyThrowsFrontBack)
{
    RingBuffer<int> rb(2);

    EXPECT_THROW(rb.front(), std::runtime_error);
    EXPECT_THROW(rb.back(), std::runtime_error);
}