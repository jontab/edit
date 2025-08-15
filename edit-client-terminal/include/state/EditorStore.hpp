#pragma once

#include "BufferSlice.hpp"
#include "ModeSlice.hpp"
#include "StatusSlice.hpp"
#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"

namespace edit::state
{

class EditorStore
{
    core::Bus<core::Event> &event_bus_;
    BufferSlice buffer_;
    ModeSlice mode_;
    StatusSlice status_;

  public:
    EditorStore(core::Bus<core::Action> &action_bus, decltype(event_bus_) event_bus);

    const BufferSlice &buffer() const;
    const ModeSlice &mode() const;
    const StatusSlice &status() const;

  private:
    void reduce(const core::actions::Insert &a);
    void reduce(const core::actions::Delete &a);
    void reduce(const core::actions::Backspace &a);
    void reduce(const core::actions::CursorUp &a);
    void reduce(const core::actions::CursorDown &a);
    void reduce(const core::actions::CursorLeft &a);
    void reduce(const core::actions::CursorRight &a);
    void reduce(const core::actions::ChangeStatus &a);
};

} // namespace edit::state
