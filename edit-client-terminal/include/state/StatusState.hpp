#pragma once

#include "core/Action.hpp"

namespace edit
{

struct StatusState
{
    std::string status;
    std::size_t command_cursor;
    std::string command_content;
};

} // namespace edit
