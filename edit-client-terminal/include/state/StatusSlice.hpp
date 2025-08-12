#pragma once

#include "core/Event.hpp"
#include "state/StatusState.hpp"
#include <optional>

namespace edit::state
{

class StatusSlice
{
    StatusState s_;

  public:
    StatusSlice();

    const StatusState &state() const;

    // Reduce.
    void reduce(const edit::core::CursorLeftAction &a);
    void reduce(const edit::core::CursorRightAction &a);
    std::optional<edit::core::CommandEnteredEvent> reduce(const edit::core::InsertAction &a);
    void reduce(const edit::core::DeleteAction &a);
    void reduce(const edit::core::BackspaceAction &a);
    void reduce(const edit::core::ChangeStatusAction &a);

    // Other.
    void reset_command();
};

} // namespace edit::state
