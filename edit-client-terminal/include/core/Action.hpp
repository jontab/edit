#pragma once

#include "Mode.hpp"
#include <cstdint>
#include <string>
#include <variant>

namespace edit
{

/**
 * @brief Request to quit the program.
 */
struct QuitAction
{
};

/**
 * @brief Leave `InsertMode` or `CommandMode`.
 */
struct EscapeAction
{
};

/**
 * @brief Attempt to invoke a command.
 */
struct CommandAction
{
    std::string text;
};

/**
 * @brief Request to change `Mode`.
 */
struct ChangeModeAction
{
    Mode to;
};

/**
 * @brief Request to insert a character.
 */
struct InsertAction
{
    std::uint32_t ch;
};

/**
 * @brief Request to delete a character.
 */
struct DeleteAction
{
};

/**
 * @brief Request to backspace (move left and delete a character).
 */
struct BackspaceAction
{
};

/**
 * @brief Request to move up.
 */
struct CursorUpAction
{
};

/**
 * @brief Request to move down.
 */
struct CursorDownAction
{
};

/**
 * @brief Request to move to the left.
 */
struct CursorLeftAction
{
};

/**
 * @brief Request to move to the right.
 */
struct CursorRightAction
{
};

using Action = std::variant<QuitAction, // Break.
    EscapeAction,
    CommandAction,
    ChangeModeAction,
    InsertAction,
    DeleteAction,
    BackspaceAction,
    CursorUpAction,
    CursorDownAction,
    CursorLeftAction,
    CursorRightAction>;

} // namespace edit
