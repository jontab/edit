#pragma once

#include <cstddef>
#include <string>

namespace edit::state
{

struct StatusState
{
    std::string status;
    std::size_t command_cursor;
    std::string command_content;
};

} // namespace edit::state
