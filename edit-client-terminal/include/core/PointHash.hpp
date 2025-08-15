#pragma once

#include "Point.hpp"
#include <cstddef>
#include <functional>

namespace std
{

template <typename T> struct hash<edit::core::Point<T>>
{
    std::size_t operator()(const edit::core::Point<T> &point) const
    {
        const auto h1 = std::hash<T>()(point.y);
        const auto h2 = std::hash<T>()(point.x);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std
