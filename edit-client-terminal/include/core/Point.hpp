#pragma once

#include <cstddef>
#include <unordered_map>

namespace edit::core
{

template <typename T> struct Point
{
    T y;
    T x;

    bool operator==(const Point &other) const
    {
        return (y == other.y) && (x == other.x);
    }

    bool operator!=(const Point &other) const
    {
        return !((*this) == other);
    }
};

} // namespace edit::core

template <typename T> struct std::hash<edit::core::Point<T>>
{
    std::size_t operator()(const edit::core::Point<T> &point) const
    {
        return std::hash<unsigned int>()(point.y) ^ (std::hash<unsigned int>()(point.x) << 1);
    }
};
