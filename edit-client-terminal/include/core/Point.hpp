#pragma once

#include <cstddef>
#include <unordered_map>

namespace edit::ui
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

} // namespace edit::ui

template <typename T> struct std::hash<edit::ui::Point<T>>
{
    std::size_t operator()(const edit::ui::Point<T> &point) const
    {
        return std::hash<unsigned int>()(point.y) ^ (std::hash<unsigned int>()(point.x) << 1);
    }
};
