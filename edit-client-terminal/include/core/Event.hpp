#pragma once

#include <edit-common/Char.hpp>
#include <variant>

namespace edit
{

struct CursorMoved
{
    std::size_t new_index;
    std::size_t new_y;
    std::size_t new_x;
};

struct CharInserted
{
    common::Char ch;
};

using Event = std::variant<CursorMoved, CharInserted>;

} // namespace edit
