/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-01-22
 */
#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
    public:

        void push(const T& value)
        {
            {
                std::scoped_lock lock(mtx_);
                queue_.push(value);
            }
            cv_.notify_one();   
        }

        void push(T&& value)
        {
            {
                std::scoped_lock lock(mtx_);
                queue_.push(std::move(value));
            }
            cv_.notify_one();
        }

        std::optional<T> try_pop() 
        {
            std::scoped_lock lock(mtx_);
            if (queue_.empty())
            {
                return std::nullopt;
            } 
            T value = std::move(queue_.front());
            queue_.pop();
            return value;
        }

        template<typename Rep, typename Period>
        std::optional<T> pop_wait(const std::chrono::duration<Rep, Period>& max_duration)
        {
            std::unique_lock<std::mutex> lock(mtx_);

            if(!queue_.empty())
            {
                T value = std::move(queue_.front());
                queue_.pop();
                return value;
            }

            cv_.wait_for(lock, max_duration);
            
            if(queue_.empty())
            {
                return std::nullopt;
            }
            T value = std::move(queue_.front());
            queue_.pop();
            return value;
        }

        bool empty() const
        {
            std::scoped_lock lock(mtx_);
            return queue_.empty();
        }
    
    private:
        mutable std::mutex mtx_;
        std::condition_variable cv_;
        std::queue<T> queue_;
};
