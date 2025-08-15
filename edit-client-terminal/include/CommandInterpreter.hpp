#pragma once

#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"

namespace edit
{

class CommandInterpreter
{
    core::Bus<core::Action> &action_bus_;

  public:
    CommandInterpreter(decltype(action_bus_) action_bus, core::Bus<core::Event> &event_bus);

  private:
    void handle(const core::events::CommandEntered &ev);
};

} // namespace edit
