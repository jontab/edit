#pragma once

#include "core/ActionTypes.hpp"
#include "core/EventTypes.hpp"
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

    void reduce(const core::actions::CursorLeft &a);
    void reduce(const core::actions::CursorRight &a);
    std::optional<core::events::CommandEntered> reduce(const core::actions::Insert &a);
    void reduce(const core::actions::Delete &a);
    void reduce(const core::actions::Backspace &a);
    void reduce(const core::actions::ChangeStatus &a);

    void reset_command();
};

} // namespace edit::state
