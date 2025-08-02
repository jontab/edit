#include "Editor.hpp"

edit::Editor::Editor(std::shared_ptr<boost::asio::io_context> ioc,
    std::shared_ptr<ActionBus> action_bus,
    std::shared_ptr<EventBus> event_bus,
    std::shared_ptr<edit::network::INetworkComponent> network_component,
    std::unique_ptr<ui::IView> &&view)
    : ioc_(ioc)
    , action_bus_(action_bus)
    , event_bus_(event_bus)
    , network_component_(network_component)
    , mode_component_(*action_bus, *event_bus)
    , buffer_component_(*action_bus, *event_bus, mode_component_)
    , status_component_(*action_bus, *event_bus, mode_component_)
    , view_(std::move(view))
    , is_running_(true)
{
    action_bus_->on<QuitAction>([this](const auto &) { is_running_ = false; });
    action_bus_->on<CommandAction>([this](const auto &ev) {
        // TODO: Move this into a command handler or something.
        if (ev.text.find("quit") != std::string::npos)
            is_running_ = false;
    });
    network_component_->bind(*action_bus, *event_bus);
}

void edit::Editor::run()
{
    while (is_running_)
    {
        ioc_->poll();
        view_->render(this->buffer_component_, this->status_component_);
        view_->poll(*action_bus_, *event_bus_);
    }
}
