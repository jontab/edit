#pragma once

#include "core/Bus.hpp"
#include "core/Mode.hpp"

namespace edit
{

class ModeComponent
{
    ActionBus &action_bus_;
    EventBus &event_bus_;
    Mode mode_;

  public:
    ModeComponent(ActionBus &action_bus, EventBus &event_bus);

    Mode mode() const;

  private:
    void handle_key_pressed(const KeyPressedEvent &ev);
    void handle_change_mode(Mode mode);
};

} // namespace edit
