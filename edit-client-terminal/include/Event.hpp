#pragma once

#include <variant>

namespace edit
{

struct CursorMoved
{
    std::size_t new_index;
    std::size_t new_y;
    std::size_t new_x;
};

using Event = std::variant<CursorMoved>;

} // namespace edit
