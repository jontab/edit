#pragma once

#include <cstdint>
#include <edit-common/Char.hpp>
#include <string>

namespace edit::core::events
{

struct CharDeleted
{
    common::Char ch;
};

struct CharInserted
{
    common::Char ch;
};

struct CommandEntered
{
    std::string text;
};

struct KeyPressed
{
    std::uint32_t ch;
};

} // namespace edit::core::events
