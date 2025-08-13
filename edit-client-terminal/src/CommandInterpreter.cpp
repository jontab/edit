#include "CommandInterpreter.hpp"

using namespace edit::core;

edit::CommandInterpreter::CommandInterpreter(Dispatcher &dispatcher)
    : dispatcher_(dispatcher)
{
    dispatcher.on_event<CommandEnteredEvent>([this](const auto &ev) { handle(ev); });
}

void edit::CommandInterpreter::handle(const CommandEnteredEvent &ev)
{
    if (ev.text.find("quit") != std::string::npos)
        dispatcher_.dispatch(QuitAction{});
    dispatcher_.dispatch(ChangeModeAction{Mode::NormalMode});
}
