#include "Editor.hpp"

using namespace edit::core;
using namespace edit::network;
using namespace edit::ui;

edit::Editor::Editor(std::shared_ptr<boost::asio::io_context> ioc,
    std::unique_ptr<Dispatcher> &&dispatcher,
    std::shared_ptr<INetworkComponent> network_component,
    std::unique_ptr<IView> &&view)
    : ioc_(ioc)
    , dispatcher_(std::move(dispatcher))
    , network_component_(network_component)
    , store_(*dispatcher_)
    , input_interpreter_(*dispatcher_, store_)
    , command_interpreter_(*dispatcher_)
    , view_(std::move(view))
    , is_running_(true)
{
    dispatcher_->on_action<QuitAction>([this](const auto &) { is_running_ = false; });
    network_component_->bind(*dispatcher_);
}

void edit::Editor::run()
{
    while (is_running_)
    {
        ioc_->poll();
        view_->render(this->store_);
        view_->poll(*dispatcher_);
    }
}
