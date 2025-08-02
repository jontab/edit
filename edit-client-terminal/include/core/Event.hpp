#pragma once

#include "Mode.hpp"
#include <edit-common/Char.hpp>
#include <variant>

namespace edit
{

/**
 * @brief The cursor has already moved.
 */
struct CursorMovedEvent
{
    std::size_t new_index;
    std::size_t new_y;
    std::size_t new_x;
};

/**
 * @brief A key has been pressed.
 */
struct KeyPressedEvent
{
    std::uint32_t ch;
};

/**
 * @brief A character has already been inserted.
 */
struct CharInsertedEvent
{
    common::Char ch;
};

/**
 * @brief The mode has already been changed.
 */
struct ModeChangedEvent
{
    Mode to;
};

using Event = std::variant<CursorMovedEvent, KeyPressedEvent, CharInsertedEvent, ModeChangedEvent>;

} // namespace edit
