#include "Editor.hpp"

using namespace edit::core;
using namespace edit::network;
using namespace edit::ui;

edit::Editor::Editor(decltype(ioc_) ioc,
    decltype(action_bus_) &&action_bus,
    decltype(event_bus_) &&event_bus,
    decltype(network_component_) network_component,
    decltype(view_) &&view)
    : ioc_(ioc)
    , action_bus_(std::move(action_bus))
    , event_bus_(std::move(event_bus))
    , ioc_thread_()
    , is_running_(true)
    , network_component_(network_component)
    , view_(std::move(view))
    , store_(*action_bus_, *event_bus_)
    , input_interpreter_(*action_bus_, *event_bus_, store_)
    , command_interpreter_(*action_bus_, *event_bus_)
{
    action_bus_->on<actions::Quit>([this](const auto &) { is_running_ = false; });
}

void edit::Editor::run()
{
    ioc_thread_ = std::thread([this] { ioc_->run(); });
    while (is_running_)
    {
        action_bus_->publish();
        event_bus_->publish();
        view_->render(this->store_);
        view_->poll(*action_bus_, *event_bus_);
    }

    ioc_->stop();
    if (ioc_thread_.joinable())
        ioc_thread_.join();
}
