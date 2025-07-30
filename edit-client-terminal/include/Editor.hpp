#pragma once

#include "BufferComponent.hpp"
#include "StatusComponent.hpp"
#include "net/INetworkComponent.hpp"
#include "ui/IView.hpp"
#include <iostream>

namespace edit
{

class Editor
{
    std::unique_ptr<ui::IView> view_;
    ActionBus action_bus_;
    EventBus event_bus_;
    BufferComponent buffer_component_;
    StatusComponent status_component_;
    std::unique_ptr<edit::net::INetworkComponent> network_component_;
    bool is_running_;

  public:
    Editor(std::unique_ptr<ui::IView> &&view, std::unique_ptr<edit::net::INetworkComponent> &&network);

    void run();
};

} // namespace edit
