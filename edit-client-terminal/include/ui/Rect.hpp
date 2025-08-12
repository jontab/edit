#pragma once

#include "core/Point.hpp"

namespace edit::ui
{

template <typename T> struct Rect
{
    edit::core::Point<T> anchor;
    edit::core::Point<T> size;
};

} // namespace edit::ui
