#pragma once

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
