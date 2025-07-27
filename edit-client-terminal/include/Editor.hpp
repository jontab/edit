#pragma once

#include "BufferComponent.hpp"
#include "StatusComponent.hpp"
#include "View.hpp"
#include <iostream>

namespace edit
{

class Editor
{
    std::unique_ptr<IView> view;
    ActionBus action_bus;
    EventBus event_bus;
    BufferComponent buffer_component;
    StatusComponent status_component;
    bool is_running;

  public:
    Editor(std::unique_ptr<IView> view);

    void run();
};

} // namespace edit
