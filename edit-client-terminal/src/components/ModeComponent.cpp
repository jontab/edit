#include "components/ModeComponent.hpp"

edit::ModeComponent::ModeComponent(ActionBus &action_bus, EventBus &event_bus)
    : action_bus_(action_bus)
    , event_bus_(event_bus)
    , mode_(Mode::NormalMode)
{
    action_bus.on<ChangeModeAction>([this](const auto &ev) { handle_change_mode(ev.to); });
    event_bus.on<KeyPressedEvent>([this](const auto &ev) { handle_key_pressed(ev); });
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
        action_bus_.publish(edit::InsertAction{ev.ch});
        break;
    }
}

void edit::ModeComponent::handle_change_mode(Mode mode)
{
    if (mode_ != mode)
    {
        mode_ = mode;
        event_bus_.publish(edit::ModeChangedEvent{mode});
    }
}
