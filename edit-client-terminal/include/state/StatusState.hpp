#pragma once

#include "core/Action.hpp"

namespace edit::state
{

struct StatusState
{
    std::string status;
    std::size_t command_cursor;
    std::string command_content;
};

} // namespace edit::state
