#include "state/EditorStore.hpp"

using namespace edit::core;
using namespace edit::state;

EditorStore::EditorStore(core::Bus<core::Action> &action_bus, decltype(event_bus_) &event_bus)
    : event_bus_(event_bus)
    , buffer_()
    , mode_()
    , status_()
{
    action_bus.on<actions::Escape>([this](const auto &a) { mode_.reduce(a); });
    action_bus.on<actions::ChangeMode>([this](const auto &a) {
        if (a.to == Mode::CommandMode)
            status_.reset_command();
        mode_.reduce(a);
    });

    action_bus.on<actions::Insert>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::Delete>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::Backspace>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::RemoteInsert>([this](const auto &a) { buffer_.reduce(a); });
    action_bus.on<actions::RemoteDelete>([this](const auto &a) { buffer_.reduce(a); });
    action_bus.on<actions::CursorUp>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::CursorDown>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::CursorLeft>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::CursorRight>([this](const auto &a) { reduce(a); });
    action_bus.on<actions::ChangeStatus>([this](const auto &a) { reduce(a); });
}

const BufferSlice &EditorStore::buffer() const
{
    return buffer_;
}

const ModeSlice &EditorStore::mode() const
{
    return mode_;
}

const StatusSlice &EditorStore::status() const
{
    return status_;
}

void EditorStore::reduce(const actions::Insert &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
        break;
    case Mode::InsertMode:
        event_bus_.post(buffer_.reduce(a));
        break;
    case Mode::CommandMode:
        if (auto ev = status_.reduce(a); ev.has_value())
            event_bus_.post(ev.value());
        break;
    }
}

void EditorStore::reduce(const actions::Delete &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
    case Mode::InsertMode:
        if (auto ev = buffer_.reduce(a); ev.has_value())
            event_bus_.post(ev.value());
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    }
}

void EditorStore::reduce(const actions::Backspace &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
        break;
    case Mode::InsertMode:
        if (auto ev = buffer_.reduce(a); ev.has_value())
            event_bus_.post(ev.value());
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    }
}

void EditorStore::reduce(const actions::CursorUp &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        // TODO: Go up in history.
        break;
    }
}

void EditorStore::reduce(const actions::CursorDown &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        // TODO: Go down in history.
        break;
    }
}

void EditorStore::reduce(const actions::CursorLeft &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    }
}

void EditorStore::reduce(const actions::CursorRight &a)
{
    switch (mode_.mode())
    {
    case Mode::NormalMode:
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    }
}

void EditorStore::reduce(const actions::ChangeStatus &a)
{
    status_.reduce(a);
}
