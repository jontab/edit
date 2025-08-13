#include "InputInterpreter.hpp"

using namespace edit::core;
using namespace edit::state;

edit::InputInterpreter::InputInterpreter(Dispatcher &dispatcher, const EditorStore &store)
    : dispatcher_(dispatcher)
    , store_(store)
{
    dispatcher.on_event<KeyPressedEvent>([this](const auto &ev) { handle(ev); });
}

void edit::InputInterpreter::handle(const KeyPressedEvent &ev)
{
    switch (store_.mode().mode())
    {
    case Mode::NormalMode:
        handle_normal_mode(ev);
        break;
    case Mode::InsertMode:
    case Mode::CommandMode:
        dispatcher_.dispatch(InsertAction{ev.ch});
        dispatcher_.dispatch(CursorRightAction{});
        break;
    default:
        break;
    }
}

void edit::InputInterpreter::handle_normal_mode(const KeyPressedEvent &ev)
{
    // TODO: Configurable key-maps!
    switch (ev.ch)
    {
    case 'i':
        dispatcher_.dispatch(ChangeModeAction{Mode::InsertMode});
        break;
    case ':':
        dispatcher_.dispatch(ChangeModeAction{Mode::CommandMode});
        break;

    case 'j':
        dispatcher_.dispatch(CursorDownAction{});
        break;
    case 'k':
        dispatcher_.dispatch(CursorUpAction{});
        break;
    case 'h':
        dispatcher_.dispatch(CursorLeftAction{});
        break;
    case 'l':
        dispatcher_.dispatch(CursorRightAction{});
        break;

    case 'x':
        dispatcher_.dispatch(DeleteAction{});
        break;
    default:
        break;
    }
}
