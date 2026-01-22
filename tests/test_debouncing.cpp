/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-01-22
 */

#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "LichensCPP/debouncing.h"
#include "LichensCPP/unused.h"

using namespace LichensCPP;

const uint64_t TOO_LONG_DELAY = from_sec(1);

TEST(Debouncing, ConstructAndBasics)
{
    int call_count = 0;
    auto debouncer = Debouncing([&call_count](const int& value) {
        UNUSED(value);
        call_count++;
    });
    UNUSED(debouncer);

    EXPECT_EQ(call_count, 0);
}

TEST(Debouncing, FirstCallExecutesImmediately)
{
    int call_count = 0;
    int received_value = 0;

    auto debouncer = Debouncing([&](const int& value) {
        call_count++;
        received_value = value;
    }, TOO_LONG_DELAY, 1u);

    debouncer.call(42);

    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(received_value, 42);
}

TEST(Debouncing, CallsWithinMaxNbCallsExecute)
{
    int call_count = 0;
    std::vector<int> received_values;

    auto debouncer = Debouncing([&](const int& value) {
        call_count++;
        received_values.push_back(value);
    }, TOO_LONG_DELAY, 5u);

    // First 5 calls should all execute
    for (int i = 0; i < 5; ++i)
    {
        debouncer.call(i);
    }

    EXPECT_EQ(call_count, 1);
    ASSERT_EQ(received_values.size(), 1u);
    EXPECT_EQ(received_values[0], 4);
}

TEST(Debouncing, CallsAfterMaxNbCallsAreDebounced)
{
    int call_count = 0;

    auto debouncer = Debouncing([&](const int& value) {
        UNUSED(value);
        call_count++;
    }, TOO_LONG_DELAY, 3u);

    debouncer.call(1);
    EXPECT_EQ(call_count, 0);
    debouncer.call(2);
    EXPECT_EQ(call_count, 0);
    debouncer.call(3);
    EXPECT_EQ(call_count, 1);
    debouncer.call(4);
    EXPECT_EQ(call_count, 1);
    debouncer.call(5);
    EXPECT_EQ(call_count, 1);
    debouncer.call(6);
    EXPECT_EQ(call_count, 2);
}

TEST(Debouncing, DebounceReleasesAfterInterval)
{
    int call_count = 0;
    int last_value = 0;

    // Short interval for faster testing
    auto debouncer = Debouncing([&](const int& value) {
        call_count++;
        last_value = value;
    }, from_usec(100), 1000u);

    debouncer.call(1);
    EXPECT_EQ(call_count, 0);
    EXPECT_EQ(last_value, 0);

    debouncer.call(2);
    EXPECT_EQ(call_count, 0);
    EXPECT_EQ(last_value, 0);

    // Wait for interval to pass
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Now the call should execute
    debouncer.call(3);
    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(last_value, 3);
}
TEST(Debouncing, ExceptionInCallbackIsHandled)
{
    int call_count = 0;

    auto debouncer = Debouncing([&](const int& value) {
        UNUSED(value);
        call_count++;
        throw std::runtime_error("Test exception");
    }, TOO_LONG_DELAY, 1u);

    EXPECT_NO_THROW(debouncer.call(1));
    EXPECT_EQ(call_count, 1);
}

TEST(Debouncing, MultipleArgumentsForwarding)
{
    int call_count = 0;
    int last_a = 0;
    float last_b = 0.0f;
    std::string last_c;

    auto debouncer = Debouncing([&](int a, float b, const std::string& c) {
        call_count++;
        last_a = a;
        last_b = b;
        last_c = c;
    }, TOO_LONG_DELAY, 1u);

    debouncer.call(42, 3.14f, "test");

    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(last_a, 42);
    EXPECT_FLOAT_EQ(last_b, 3.14f);
    EXPECT_EQ(last_c, "test");
}