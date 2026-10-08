/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Wed Oct 07 2026
 */

#pragma once

#include <version>
#include <string>

#if defined(__ANDROID__)
    #include <android/api-level.h>
    
    // NDK r22+ officially supports std::filesystem. However, if your minSdkVersion 
    // targets legacy versions (below API 24), it is safer to stick to the shim.
    #if defined(__ANDROID_API__) && __ANDROID_API__ < 24
        #define LICHENS_CPP_FORCE_FILESYSTEM_FALLBACK 1
    #endif
#endif

#if !defined(LICHENSCPP_FORCE_FILESYSTEM_FALLBACK) && __cplusplus >= 201703L
    #define LICHENS_CPP_HAS_STD_FILESYSTEM 1
    #include <filesystem>
#else
    #define LICHENS_CPP_HAS_STD_FILESYSTEM 0
    #include <ghc/filesystem.hpp>
#endif

namespace LichensCPP
{
#if LICHENS_CPP_HAS_STD_FILESYSTEM == 1
    namespace filesystem = std::filesystem;
#else
    namespace filesystem = ghc::filesystem;
#endif
} // namespace LichensCPP
