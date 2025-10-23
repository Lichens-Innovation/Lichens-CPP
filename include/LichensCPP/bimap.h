/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Thu Oct 23 2025
 */

#pragma once

#include <type_traits>
#include <unordered_map>

namespace LichensCPP
{

    template <typename T1, typename T2>
    class BiMap
    {
    public:
        using iterator = typename std::unordered_map<T1, T2>::iterator;
        using const_iterator = typename std::unordered_map<T1, T2>::const_iterator;

        BiMap() = default;
        ~BiMap() = default;

        BiMap(const BiMap &other) = default;
        BiMap(BiMap &&other) noexcept = default;
        BiMap &operator=(const BiMap &other) = default;
        BiMap &operator=(BiMap &&other) noexcept = default;

        BiMap(std::initializer_list<std::pair<const T1, T2>> init)
        {
            for (const auto &kv : init)
            {
                insert(kv.first, kv.second);
            }
        }

        bool insert(const T1 &key, const T2 &value)
        {
            if (forward_map.find(key) != forward_map.end() || reverse_map.find(value) != reverse_map.end())
            {
                return false; // Key already exists
            }
            forward_map[key] = value;
            reverse_map[value] = key;
            return true;
        }

        bool erase_by_key(const T1 &key)
        {
            auto it = forward_map.find(key);
            if (it == forward_map.end())
            {
                return false; // Key not found
            }
            reverse_map.erase(it->second);
            forward_map.erase(it);
            return true;
        }

        bool erase_by_value(const T2 &value)
        {
            auto it = reverse_map.find(value);
            if (it == reverse_map.end())
            {
                return false; // Value not found
            }
            forward_map.erase(it->second);
            reverse_map.erase(it);
            return true;
        }

        const T2 &at_key(const T1 &key) const
        {
            return forward_map.at(key);
        }
        const T1 &at_value(const T2 &value) const
        {
            return reverse_map.at(value);
        }

        bool contains_key(const T1 &key) const
        {
            return forward_map.count(key);
        }
        bool contains_value(const T2 &value) const
        {
            return reverse_map.count(value);
        }

        size_t size() const
        {
            return forward_map.size();
        }

        operator size_t() const
        {
            return forward_map.size();
        }

        void reserve(size_t n)
        {
            forward_map.reserve(n);
            reverse_map.reserve(n);
        }

        T2 operator[](const T1 &key) const
        {
            return forward_map.at(key);
        }

        template <typename U = T2, typename = std::enable_if_t<!std::is_same_v<T1, U>>>
        T1 operator[](const T2 &value) const
        {
            return reverse_map.at(value);
        }

        iterator begin() noexcept
        {
            return forward_map.begin();
        }

        iterator end() noexcept
        {
            return forward_map.end();
        }

        const_iterator cbegin() const noexcept
        {
            return forward_map.cbegin();
        }

        const_iterator cend() const noexcept
        {
            return forward_map.cend();
        }

    private:
        std::unordered_map<T1, T2> forward_map;
        std::unordered_map<T2, T1> reverse_map;
    };

} // namespace LichensCPP
