/*
 * Created on Wed Oct 14 2026
 *
 * Copyright (c) 2026 Lichens
 */

#pragma once

// Designated initializers (`a{ .b = 32, .c = 'y' }`) are only standard since C++20.
// LIL(member) lets the same aggregate initialization compile on both:
//
//     struct a { int b; char c; };
//     a value{ LIL(.b) 32, LIL(.c) 'y' };
//
// C++20 and up expands to `a value{ .b = 32, .c = 'y' }`, so the compiler checks the member names.
// Before C++20 it expands to `a value{ 32, 'y' }`, a plain positional initialization.
//
// Since the older expansion is positional, members must be listed in declaration order and none can be
// skipped except at the end (C++20 also requires declaration order, but does allow skipping).

// MSVC only reports the real language version in __cplusplus with /Zc:__cplusplus, _MSVC_LANG is always correct
#ifdef _MSVC_LANG
    #define LICHENS_CPP_INIT_LIST_CPLUSPLUS _MSVC_LANG
#else
    #define LICHENS_CPP_INIT_LIST_CPLUSPLUS __cplusplus
#endif

#if LICHENS_CPP_INIT_LIST_CPLUSPLUS >= 202002L
    #define LICHENS_CPP_HAS_DESIGNATED_INITIALIZERS 1
    #define LIL(member) member =
#else
    #define LICHENS_CPP_HAS_DESIGNATED_INITIALIZERS 0
    #define LIL(member)
#endif
