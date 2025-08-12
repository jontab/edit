#pragma once

#include "Mode.hpp"
#include <edit-common/Char.hpp>
#include <variant>

namespace edit::core
{

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
 * @brief A character has already been deleted.
 */
struct CharDeletedEvent
{
    common::Char ch;
};

/**
 * @brief A command has already been entered.
 */
struct CommandEnteredEvent
{
    std::string text;
};

using Event = std::variant<KeyPressedEvent, // Break.
    CharInsertedEvent,
    CharDeletedEvent,
    CommandEnteredEvent>;

} // namespace edit::core
