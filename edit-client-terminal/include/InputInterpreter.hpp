#pragma once

#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"
#include "state/EditorStore.hpp"

namespace edit
{

class InputInterpreter
{
    core::Bus<core::Action> &action_bus_;
    const state::EditorStore &store_;

  public:
    InputInterpreter(decltype(action_bus_) action_bus,
        core::Bus<core::Event> &event_bus,
        const state::EditorStore &store);

  private:
    void handle(const core::events::KeyPressed &ev);
    void handle_normal_mode(const core::events::KeyPressed &ev);
};

} // namespace edit
