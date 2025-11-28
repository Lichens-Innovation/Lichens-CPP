/*
 * Created on Mon Oct 20 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#pragma once

#include <initializer_list>

#define UNUSED(x) (void)(x)

template <class T, class... Ts>
inline void unused_list(const T &t, const Ts &...ts)
{
    UNUSED(t);
    unused_list(ts...);
}

#define UNUSED_LIST(...) unused_list(__VA_ARGS__)
