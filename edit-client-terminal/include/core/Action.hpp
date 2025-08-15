#pragma once

#include "ActionTypes.hpp"
#include <variant>

namespace edit::core
{

using Action = std::variant<actions::Backspace,
    actions::ChangeMode,
    actions::ChangeStatus,
    actions::Command,
    actions::CursorDown,
    actions::CursorLeft,
    actions::CursorRight,
    actions::CursorUp,
    actions::Delete,
    actions::Escape,
    actions::Insert,
    actions::RemoteDelete,
    actions::RemoteInsert,
    actions::Quit>;

} // namespace edit::core
