#include "CommandInterpreter.hpp"
#include "core/ActionTypes.hpp"

using namespace edit::core;

edit::CommandInterpreter::CommandInterpreter(decltype(action_bus_) action_bus, core::Bus<core::Event> &event_bus)
    : action_bus_(action_bus)
{
    event_bus.on<events::CommandEntered>([this](const auto &ev) { handle(ev); });
}

void edit::CommandInterpreter::handle(const events::CommandEntered &ev)
{
    if (ev.text.find("quit") != std::string::npos)
        action_bus_.post(actions::Quit{});
    action_bus_.post(actions::ChangeMode{Mode::NormalMode});
}
