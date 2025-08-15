#pragma once

#include "core/Point.hpp"

namespace edit::ui
{

template <typename T> struct Rect
{
    core::Point<T> anchor;
    core::Point<T> size;
};

} // namespace edit::ui
