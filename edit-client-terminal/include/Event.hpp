#pragma once

#include <variant>

namespace edit
{

struct CursorMoved
{
    int new_index;
    int new_y;
    int new_x;
};

using Event = std::variant<CursorMoved>;

} // namespace edit
