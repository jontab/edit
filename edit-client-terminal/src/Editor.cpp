#include "Editor.hpp"

edit::Editor::Editor(std::shared_ptr<boost::asio::io_context> ioc,
    std::unique_ptr<Dispatcher> &&dispatcher,
    std::shared_ptr<edit::network::INetworkComponent> network_component,
    std::unique_ptr<ui::IView> &&view)
    : ioc_(ioc)
    , dispatcher_(std::move(dispatcher))
    , network_component_(network_component)
    , mode_component_(*dispatcher_)
    , buffer_component_(*dispatcher_, mode_component_)
    , status_component_(*dispatcher_, mode_component_)
    , view_(std::move(view))
    , is_running_(true)
{
    dispatcher_->on_action<QuitAction>([this](const auto &) { is_running_ = false; });
    dispatcher_->on_action<CommandAction>([this](const auto &ev) {
        // TODO: Move this into a command handler or something.
        if (ev.text.find("quit") != std::string::npos)
            is_running_ = false;
    });
    network_component_->bind(*dispatcher_);
}

void edit::Editor::run()
{
    while (is_running_)
    {
        ioc_->poll();
        view_->render(this->buffer_component_, this->status_component_);
        view_->poll(*dispatcher_);
    }
}
