#include "components/ModeComponent.hpp"

edit::ModeComponent::ModeComponent(Dispatcher &dispatcher)
    : dispatcher_(dispatcher)
    , mode_(Mode::NormalMode)
{
    dispatcher.on_action<ChangeModeAction>([this](const auto &ev) { handle_change_mode(ev.to); });
    dispatcher.on_event<KeyPressedEvent>([this](const auto &ev) { handle_key_pressed(ev); });
}

edit::Mode edit::ModeComponent::mode() const
{
    return mode_;
}

void edit::ModeComponent::handle_key_pressed(const KeyPressedEvent &ev)
{
    switch (mode_)
    {
    case Mode::NormalMode:
        // TODO: Introduce some sort of input translation layer so we don't have to do this. The problem is when you
        //       go from `Normal` to `Insert` via 'i', the buffer also inserts the 'i'.
        if (ev.ch == 'i')
            handle_change_mode(Mode::InsertMode);
        if (ev.ch == ':')
            handle_change_mode(Mode::CommandMode);
        break;

    default:
        dispatcher_.dispatch(edit::InsertAction{ev.ch});
        break;
    }
}

void edit::ModeComponent::handle_change_mode(Mode mode)
{
    if (mode_ != mode)
    {
        mode_ = mode;
        dispatcher_.emit(edit::ModeChangedEvent{mode});
    }
}
