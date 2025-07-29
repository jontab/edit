#pragma once

#include "Point.hpp"

namespace edit::ui
{

template <typename T> struct Rect
{
    Point<T> anchor;
    Point<T> size;
};

} // namespace edit::ui
