/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Fri Jul 11 2025
 */

#pragma once

#include <version>

#if __cpp_lib_expected >= 202202L
    #include <expected>
#else
    #include "LichensCPP/expected.hpp"
#endif

namespace LichensCPP 
{

// Use std::expected if available, otherwise fall back to tl::expected as shim
#if __cpp_lib_expected >= 202202L
    template <class T, class E>
    using expected = std::expected<T, E>;

    template <class E>
    using unexpected = std::unexpected<E>;
#else
    template <class T, class E>
    using expected = tl::expected<T, E>;

    template <class E>
    using unexpected = tl::unexpected<E>;
#endif

} // namespace LichensCPP
