#pragma once

#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"
#include "state/EditorStore.hpp"

namespace edit::ui
{

class IView
{
  public:
    virtual ~IView() = default;

    virtual void poll(core::Bus<core::Action> &action_bus, core::Bus<core::Event> &event_bus) = 0;
    virtual void render(const state::EditorStore &store) = 0;
};

} // namespace edit::ui
