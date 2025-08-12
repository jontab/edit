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
    , view_(std::move(view))
    , is_running_(true)
{
    dispatcher_->on_action<QuitAction>([this](const auto &) { is_running_ = false; });
    dispatcher_->on_event<CommandEnteredEvent>([this](const auto &ev) { handle(ev); });
    dispatcher_->on_event<KeyPressedEvent>([this](const auto &ev) { handle(ev); });
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

void edit::Editor::handle(const CommandEnteredEvent &e)
{
    if (e.text.find("quit") != std::string::npos)
        is_running_ = false;
    dispatcher_->dispatch(ChangeModeAction{Mode::NormalMode});
}

void edit::Editor::handle(const KeyPressedEvent &e)
{
    if (store_.mode().mode() == Mode::NormalMode)
    {
        if (e.ch == 'i')
            dispatcher_->dispatch(ChangeModeAction{Mode::InsertMode});
        if (e.ch == ':')
        {
            dispatcher_->dispatch(ChangeModeAction{Mode::CommandMode});
        }
    }
    else
    {
        dispatcher_->dispatch(InsertAction{e.ch});
    }
}
