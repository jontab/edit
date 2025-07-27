#pragma once

#include <variant>

namespace edit
{

struct Quit
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

using Action = std::variant<Quit, CursorUp, CursorDown, CursorLeft, CursorRight>;

} // namespace edit
