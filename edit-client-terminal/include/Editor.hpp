#pragma once

#include "components/BufferComponent.hpp"
#include "components/StatusComponent.hpp"
#include "network/INetworkComponent.hpp"
#include "ui/IView.hpp"
#include <boost/asio.hpp>

namespace edit
{

class Editor
{
    // Core.
    std::shared_ptr<boost::asio::io_context> ioc_;
    std::shared_ptr<ActionBus> action_bus_;
    std::shared_ptr<EventBus> event_bus_;

    // Components.
    std::shared_ptr<edit::network::INetworkComponent> network_component_;
    ModeComponent mode_component_;
    BufferComponent buffer_component_;
    StatusComponent status_component_;

    // View.
    std::unique_ptr<ui::IView> view_;

    // State.
    bool is_running_;

  public:
    Editor(std::shared_ptr<boost::asio::io_context> ioc,
        std::shared_ptr<ActionBus> action_bus,
        std::shared_ptr<EventBus> event_bus,
        std::shared_ptr<edit::network::INetworkComponent> network_component,
        std::unique_ptr<ui::IView> &&view);

    void run();
};

} // namespace edit
