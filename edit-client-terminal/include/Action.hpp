#pragma once

#include <cstdint>
#include <variant>

namespace edit
{

struct Quit
{
};

struct Insert
{
    std::uint32_t ch;
};

struct Delete
{
};

struct Backspace
{
};

struct CursorUp
{
};

struct CursorDown
{
};

struct CursorLeft
{
};

struct CursorRight
{
};

using Action = std::variant<Quit, Insert, Delete, Backspace, CursorUp, CursorDown, CursorLeft, CursorRight>;

} // namespace edit
