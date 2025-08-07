#pragma once

#include "state/StatusState.hpp"
#include "core/Event.hpp"
#include <optional>

namespace edit
{

class StatusSlice
{
    StatusState s_;

  public:
    StatusSlice();

    // Reduce.
    void reduce(const CursorLeftAction &a);
    void reduce(const CursorRightAction &a);
    std::optional<CommandEnteredEvent> reduce(const InsertAction &a);
    void reduce(const DeleteAction &a);
    void reduce(const BackspaceAction &a);

    // Other.
    void reset_command();
    const std::string &status() const;
    std::size_t command_cursor() const;
    const std::string &command_content() const;
};

} // namespace edit
