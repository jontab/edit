#include "Editor.hpp"

edit::Editor::Editor(std::unique_ptr<edit::ui::IView> &&view, std::unique_ptr<edit::net::INetworkComponent> &&network)
    : view_(std::move(view)), action_bus_(), event_bus_(), buffer_component_(this->action_bus_, this->event_bus_),
      status_component_(this->event_bus_), network_component_(std::move(network)), is_running_(true)
{
    action_bus_.on<Quit>([this](const auto &) { is_running_ = false; });
}

void edit::Editor::run()
{
    while (is_running_)
    {
        view_->render(this->buffer_component_, this->status_component_);
        view_->poll(action_bus_);
    }
}
