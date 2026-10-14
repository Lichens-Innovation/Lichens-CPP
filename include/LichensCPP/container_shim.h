/*
 * Created on Wed Oct 14 2026
 *
 * Copyright (c) 2026 Lichens
 */

#pragma once

namespace LichensCPP
{

/// @brief TO use contains on some container prior to C++, like std::unordored_set::contains
/// @tparam Container like std::unordored_set
/// @tparam Key   the template of container
/// @param c the actual container
/// @param k the key that should be checked
/// @return 
template <typename Container, typename Key>
bool contains(const Container& c, const Key& k)
{
    #if __cplusplus > 201703L
        return c.contains(k);
    #else
        return c.find(k) != c.end();
    #endif
}

} // namespace LichensCPP
 