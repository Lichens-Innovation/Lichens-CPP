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

#if __cpp_lib_math_constants >= 201907L
    #include <numbers>
#endif

namespace LichensCPP::numbers
{

// Use std::numbers if available, otherwise fall back to our own constants as shim
#if __cpp_lib_math_constants >= 201907L
    #define LICHENS_CPP_HAS_STD_NUMBER 1
    using std::numbers::e_v;
    using std::numbers::log2e_v;
    using std::numbers::log10e_v;
    using std::numbers::pi_v;
    using std::numbers::inv_pi_v;
    using std::numbers::inv_sqrtpi_v;
    using std::numbers::ln2_v;
    using std::numbers::ln10_v;
    using std::numbers::sqrt2_v;
    using std::numbers::sqrt3_v;
    using std::numbers::inv_sqrt3_v;
    using std::numbers::egamma_v;
    using std::numbers::phi_v;

    using std::numbers::e;
    using std::numbers::log2e;
    using std::numbers::log10e;
    using std::numbers::pi;
    using std::numbers::inv_pi;
    using std::numbers::inv_sqrtpi;
    using std::numbers::ln2;
    using std::numbers::ln10;
    using std::numbers::sqrt2;
    using std::numbers::sqrt3;
    using std::numbers::inv_sqrt3;
    using std::numbers::egamma;
    using std::numbers::phi;
#else
    #define LICHENS_CPP_HAS_STD_NUMBER 0
    // Same values as libc++/libstdc++ <numbers>
    template <class T> inline constexpr T e_v          = static_cast<T>(2.718281828459045235360287471352662L);
    template <class T> inline constexpr T log2e_v      = static_cast<T>(1.442695040888963407359924681001892L);
    template <class T> inline constexpr T log10e_v     = static_cast<T>(0.434294481903251827651128918916605L);
    template <class T> inline constexpr T pi_v         = static_cast<T>(3.141592653589793238462643383279502L);
    template <class T> inline constexpr T inv_pi_v     = static_cast<T>(0.318309886183790671537767526745028L);
    template <class T> inline constexpr T inv_sqrtpi_v = static_cast<T>(0.564189583547756286948079451560772L);
    template <class T> inline constexpr T ln2_v        = static_cast<T>(0.693147180559945309417232121458176L);
    template <class T> inline constexpr T ln10_v       = static_cast<T>(2.302585092994045684017991454684364L);
    template <class T> inline constexpr T sqrt2_v      = static_cast<T>(1.414213562373095048801688724209698L);
    template <class T> inline constexpr T sqrt3_v      = static_cast<T>(1.732050807568877293527446341505872L);
    template <class T> inline constexpr T inv_sqrt3_v  = static_cast<T>(0.577350269189625764509148780501957L);
    template <class T> inline constexpr T egamma_v     = static_cast<T>(0.577215664901532860606512090082402L);
    template <class T> inline constexpr T phi_v        = static_cast<T>(1.618033988749894848204586834365638L);

    inline constexpr double e          = e_v<double>;
    inline constexpr double log2e      = log2e_v<double>;
    inline constexpr double log10e     = log10e_v<double>;
    inline constexpr double pi         = pi_v<double>;
    inline constexpr double inv_pi     = inv_pi_v<double>;
    inline constexpr double inv_sqrtpi = inv_sqrtpi_v<double>;
    inline constexpr double ln2        = ln2_v<double>;
    inline constexpr double ln10       = ln10_v<double>;
    inline constexpr double sqrt2      = sqrt2_v<double>;
    inline constexpr double sqrt3      = sqrt3_v<double>;
    inline constexpr double inv_sqrt3  = inv_sqrt3_v<double>;
    inline constexpr double egamma     = egamma_v<double>;
    inline constexpr double phi        = phi_v<double>;
#endif

} // namespace LichensCPP::numbers
