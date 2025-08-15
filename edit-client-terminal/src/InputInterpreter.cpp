#include "InputInterpreter.hpp"

using namespace edit::core;
using namespace edit::state;

edit::InputInterpreter::InputInterpreter(decltype(action_bus_) action_bus,
    core::Bus<core::Event> &event_bus,
    const EditorStore &store)
    : action_bus_(action_bus)
    , store_(store)
{
    event_bus.on<events::KeyPressed>([this](const auto &ev) { handle(ev); });
}

void edit::InputInterpreter::handle(const events::KeyPressed &ev)
{
    switch (store_.mode().mode())
    {
    case Mode::NormalMode:
        handle_normal_mode(ev);
        break;
    case Mode::InsertMode:
    case Mode::CommandMode:
        action_bus_.post(actions::Insert{ev.ch});
        action_bus_.post(actions::CursorRight{});
        break;
    default:
        break;
    }
}

void edit::InputInterpreter::handle_normal_mode(const events::KeyPressed &ev)
{
    // TODO: Configurable key-maps!
    switch (ev.ch)
    {
    case 'i':
        action_bus_.post(actions::ChangeMode{Mode::InsertMode});
        break;
    case ':':
        action_bus_.post(actions::ChangeMode{Mode::CommandMode});
        break;

    case 'j':
        action_bus_.post(actions::CursorDown{});
        break;
    case 'k':
        action_bus_.post(actions::CursorUp{});
        break;
    case 'h':
        action_bus_.post(actions::CursorLeft{});
        break;
    case 'l':
        action_bus_.post(actions::CursorRight{});
        break;

    case 'x':
        action_bus_.post(actions::Delete{});
        break;
    default:
        break;
    }
}
