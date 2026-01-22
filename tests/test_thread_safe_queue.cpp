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
#include <vector>
#include <chrono>
#include "LichensCPP/thread_safe_queue.h"

using namespace std::chrono_literals;

TEST(ThreadSafeQueueTest, InitiallyEmpty)
{
    ThreadSafeQueue<int> queue;
    EXPECT_TRUE(queue.empty());
}

TEST(ThreadSafeQueueTest, PushAndTryPop)
{
    ThreadSafeQueue<int> queue;

    queue.push(42);
    EXPECT_FALSE(queue.empty());

    auto value = queue.try_pop();
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 42);
    EXPECT_TRUE(queue.empty());
}

TEST(ThreadSafeQueueTest, TryPopFromEmptyQueue)
{
    ThreadSafeQueue<int> queue;

    auto value = queue.try_pop();
    EXPECT_FALSE(value.has_value());
}

TEST(ThreadSafeQueueTest, PushMultipleItems)
{
    ThreadSafeQueue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);

    EXPECT_FALSE(queue.empty());

    auto val1 = queue.try_pop();
    ASSERT_TRUE(val1.has_value());
    EXPECT_EQ(val1.value(), 1);

    auto val2 = queue.try_pop();
    ASSERT_TRUE(val2.has_value());
    EXPECT_EQ(val2.value(), 2);

    auto val3 = queue.try_pop();
    ASSERT_TRUE(val3.has_value());
    EXPECT_EQ(val3.value(), 3);

    EXPECT_TRUE(queue.empty());
}

TEST(ThreadSafeQueueTest, FIFOOrder)
{
    ThreadSafeQueue<int> queue;

    for (int i = 0; i < 10; ++i)
        queue.push(i);

    for (int i = 0; i < 10; ++i)
    {
        auto value = queue.try_pop();
        ASSERT_TRUE(value.has_value());
        EXPECT_EQ(value.value(), i);
    }
}

TEST(ThreadSafeQueueTest, PopWaitTimeout)
{
    ThreadSafeQueue<int> queue;

    auto start = std::chrono::steady_clock::now();
    auto value = queue.pop_wait(100ms);
    auto duration = std::chrono::steady_clock::now() - start;

    EXPECT_FALSE(value.has_value());
    EXPECT_GE(duration, 100ms);
}

TEST(ThreadSafeQueueTest, PopWaitSuccess)
{
    ThreadSafeQueue<int> queue;

    std::thread producer([&queue]() {
        std::this_thread::sleep_for(50ms);
        queue.push(99);
    });

    auto value = queue.pop_wait(200ms);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 99);

    producer.join();
}

TEST(ThreadSafeQueueTest, PopWaitWithDifferentDurations)
{
    ThreadSafeQueue<int> queue;

    // Test with seconds
    auto value1 = queue.pop_wait(std::chrono::seconds(0));
    EXPECT_FALSE(value1.has_value());

    // Test with milliseconds
    auto value2 = queue.pop_wait(std::chrono::milliseconds(10));
    EXPECT_FALSE(value2.has_value());

    // Test with microseconds
    auto value3 = queue.pop_wait(std::chrono::microseconds(1000));
    EXPECT_FALSE(value3.has_value());
}

TEST(ThreadSafeQueueTest, MultithreadedPushPop)
{
    ThreadSafeQueue<int> queue;
    const int num_items_per_producer = 100;
    const int num_producers = 4;
    const int total_items = num_items_per_producer * num_producers;

    std::vector<std::thread> producers;
    std::vector<int> consumed_values;
    consumed_values.reserve(total_items);

    // Start producers
    for (int t = 0; t < num_producers; ++t)
    {
        producers.emplace_back([&, t]()
        {
            const int start_number = num_items_per_producer * t;
            for (int i = 0; i < num_items_per_producer; ++i)
            {
                queue.push(start_number + i);
                int var_delay = i % 4;
                if(var_delay > 0)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(var_delay));
                }
            }
        });
    }

    // Start consumers
    std::thread consumer([&]()
    {
        for (int i = 0; i < total_items; ++i)
        {
            auto value = queue.pop_wait(1ms);
            if (value.has_value())
            {
                consumed_values.push_back(value.value());
            }
        }
    });

    // Wait for all threads
    for (auto& t : producers)
    {
        t.join();
    }
    consumer.join();

    // Verify all items were consumed
    ASSERT_EQ(consumed_values.size(), static_cast<size_t>(total_items));

    // Verify no duplicates and all values present
    std::sort(consumed_values.begin(), consumed_values.end());
    for (int i = 0; i < total_items; ++i)
    {
        EXPECT_EQ(consumed_values[i], i);
    }
}

TEST(ThreadSafeQueueTest, ConcurrentPushAndEmpty)
{
    ThreadSafeQueue<int> queue;
    std::atomic<bool> stop{false};

    std::thread pusher([&]()
    {
        for (int i = 0; i < 100 && !stop; ++i)
        {
            queue.push(i);
            std::this_thread::sleep_for(1ms);
        }
    });

    std::thread checker([&]()
    {
        for (int i = 0; i < 100; ++i)
        {
            // Just check empty() without crashing
            volatile bool is_empty = queue.empty();
            (void)is_empty;
            std::this_thread::sleep_for(1ms);
        }
        stop = true;
    });

    pusher.join();
    checker.join();

    // Drain the queue
    while (!queue.empty())
        queue.try_pop();
}

TEST(ThreadSafeQueueTest, StressTest)
{
    ThreadSafeQueue<int> queue;
    const int operations_per_thread = 10000;
    std::atomic<int> push_count{0};
    std::atomic<int> pop_count{0};

    auto pusher = [&]()
    {
        for (int i = 0; i < operations_per_thread; ++i)
        {
            queue.push(i);
            ++push_count;
        }
    };

    auto popper = [&]()
    {
        while (pop_count < operations_per_thread)
        {
            auto value = queue.pop_wait(10ms);
            if (value.has_value())
                ++pop_count;
        }
    };

    std::thread t1(pusher);
    std::thread t2(popper);

    t1.join();
    t2.join();

    EXPECT_EQ(push_count, operations_per_thread);
    EXPECT_EQ(pop_count, operations_per_thread);
    EXPECT_TRUE(queue.empty());
}

TEST(ThreadSafeQueueTest, MoveOnlyType)
{
    ThreadSafeQueue<std::unique_ptr<int>> queue;

    queue.push(std::make_unique<int>(42));

    auto value = queue.try_pop();
    ASSERT_TRUE(value.has_value());
    ASSERT_NE(value.value(), nullptr);
    EXPECT_EQ(*value.value(), 42);
}

TEST(ThreadSafeQueueTest, ProducerConsumerPattern)
{
    ThreadSafeQueue<int> queue;
    const int num_items = 100;
    std::atomic<bool> done{false};

    // Producer thread
    std::thread producer([&queue, &done, num_items]() {
        for (int i = 0; i < num_items; ++i)
        {
            queue.push(i);
            std::this_thread::sleep_for(1ms);
        }
        done = true;
    });

    // Consumer thread
    std::vector<int> results;
    std::thread consumer([&queue, &done, &results]() {
        while (!done || !queue.empty())
        {
            auto value = queue.pop_wait(50ms);
            if (value.has_value())
                results.push_back(value.value());
        }
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(results.size(), static_cast<size_t>(num_items));

    // Verify order
    for (int i = 0; i < num_items; ++i)
        EXPECT_EQ(results[i], i);
}
