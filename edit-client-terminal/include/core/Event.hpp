#pragma once

#include "EventTypes.hpp"
#include <variant>

namespace edit::core
{

using Event = std::variant<events::CharDeleted, // Break.
    events::CharInserted,
    events::CommandEntered,
    events::KeyPressed>;

} // namespace edit::core
