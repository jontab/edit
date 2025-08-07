#pragma once

#include "Point.hpp"

namespace edit::core
{

template <typename T> struct Rect
{
    Point<T> anchor;
    Point<T> size;
};

} // namespace edit::core
