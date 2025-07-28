#pragma once

#include <cstddef>

namespace edit
{

struct Point
{
    std::size_t y;
    std::size_t x;

    Point(decltype(y) y, decltype(x) x);

    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;
};

} // namespace edit
