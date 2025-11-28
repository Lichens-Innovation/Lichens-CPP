/*
 * Created on Fri Nov 28 2025
 *
 * Copyright (c) 2025 Lichens
 */

#pragma once

#include <vector>
#include <stdexcept>
#include <cstddef>

namespace LichensCPP
{

template<typename T, bool IsConst>
class RingIterator 
{
public:
    using buffer_type     = std::conditional_t<IsConst, const std::vector<T>, std::vector<T>>;
    using value_type      = T;
    using reference       = std::conditional_t<IsConst, const T&, T&>;
    using pointer         = std::conditional_t<IsConst, const T*, T*>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    RingIterator(
        buffer_type* buf,
        std::size_t capacity,
        std::size_t head,
        std::size_t size,
        std::size_t index)
        : buf_(buf),
            capacity_(capacity),
            head_(head),
            size_(size),
            index_(index)
    {}

    reference operator*() const
    {
        std::size_t pos = (head_ + capacity_ - size_ + index_) % capacity_;
        return (*buf_)[pos];
    }

    pointer operator->() const 
    { 
        return &(**this);
    }

    RingIterator& operator++()
    {
        ++index_;
        return *this;
    }

    RingIterator operator++(int)
    {
        RingIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const RingIterator& other) const
    {
        return index_ == other.index_ &&
                buf_ == other.buf_;
    }

    bool operator!=(const RingIterator& other) const
    {
        return !(*this == other);
    }

private:
    buffer_type* buf_;
    std::size_t capacity_;
    std::size_t head_;
    std::size_t size_;
    std::size_t index_;
};

template<typename T>
class RingBuffer
{
public:
    using iterator = RingIterator<T, false>;
    using const_iterator = RingIterator<T, true>;

    explicit RingBuffer(std::size_t capacity)
        : capacity_(capacity),
          buffer_(capacity),
          head_(0u),
          size_(0u)
    {
        if (capacity == 0u) 
        {
            throw std::invalid_argument("RingBuffer capacity must be > 0");
        }
    }
    

    // Push value; overwrites oldest when full
    void push(const T& value)
    {
        buffer_[head_] = value;
        head_ = (head_ + 1u) % capacity_;

        if (size_ < capacity_)
        {
            ++size_;
        }
    }

    // Same as push(T const&)
    void push(T&& value)
    {
        buffer_[head_] = std::move(value);
        head_ = (head_ + 1u) % capacity_;

        if (size_ < capacity_)
        {
            ++size_;
        }
    }

    // Access by 0 = oldest, size()-1 = newest
    T& operator[](std::size_t index)
    {
        if (index >= size_)
        { 
            throw std::out_of_range("RingBuffer index out of range"); 
        }
        std::size_t pos = (head_ + capacity_ - size_ + index) % capacity_;
        return buffer_[pos];
    }

    const T& operator[](size_t index) const 
    {
        if (index >= size_) 
        { 
            throw std::out_of_range("RingBuffer index out of range");
        }
        std::size_t pos = (head_ + capacity_ - size_ + index) % capacity_;
        return buffer_[pos];
    }

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0u; }
    bool full() const noexcept { return size_ == capacity_; }

    // Returns newest element
    T& back()
    {
        if (empty())
        {
            throw std::runtime_error("RingBuffer is empty");
        }
        std::size_t pos = (head_ + capacity_ - 1u) % capacity_;
        return buffer_[pos];
    }

    const T& back() const
    {
        if (empty())
        {
            throw std::runtime_error("RingBuffer is empty");
        }
        std::size_t pos = (head_ + capacity_ - 1u) % capacity_;
        return buffer_[pos];
    }

    // Returns oldest element
    T& front() 
    {
        if (empty())
        {
            throw std::runtime_error("RingBuffer is empty");
        }
        std::size_t pos = (head_ + capacity_ - size_) % capacity_;
        return buffer_[pos];
    }

    const T& front() const
    {
        if (empty())
        {
            throw std::runtime_error("RingBuffer is empty");
        }
        std::size_t pos = (head_ + capacity_ - size_) % capacity_;
        return buffer_[pos];
    }

    iterator begin() 
    {
        return iterator(&buffer_, capacity_, head_, size_, 0u);
    }

    iterator end()
    {
        return iterator(&buffer_, capacity_, head_, size_, size_);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }

    const_iterator cbegin() const
    {
        return const_iterator(&buffer_, capacity_, head_, size_, 0u);
    }

    const_iterator cend() const
    {
        return const_iterator(&buffer_, capacity_, head_, size_, size_);
    }

private:
    size_t capacity_;
    std::vector<T> buffer_;
    std::size_t head_;   // index where next element will be written
    std::size_t size_;   // number of elements stored (<= capacity)
};

} // namespace LichensCPP
