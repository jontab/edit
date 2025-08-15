#pragma once

#include "Mode.hpp"
#include <cstdint>
#include <edit-common/Char.hpp>
#include <string>

namespace edit::core::actions
{

struct Backspace
{
};

struct ChangeMode
{
    Mode to;
};

struct ChangeStatus
{
    std::string message;
};

struct Command
{
    std::string text;
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

struct CursorUp
{
};

struct Delete
{
};

struct Escape
{
};

struct Insert
{
    std::uint32_t ch;
};

struct RemoteDelete
{
    common::Char ch;
};

struct RemoteInsert
{
    common::Char ch;
};

struct Quit
{
};

} // namespace edit::core::actions
