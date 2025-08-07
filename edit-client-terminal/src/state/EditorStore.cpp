#include "state/EditorStore.hpp"

edit::EditorStore::EditorStore(Dispatcher &dispatcher)
    : dispatcher_(dispatcher)
    , buffer_()
    , mode_()
    , status_()
{
    dispatcher.on_action<EscapeAction>([this](const auto &a) { mode_.reduce(a); });
    dispatcher.on_action<ChangeModeAction>([this](const auto &a) {
        if (a.to == Mode::CommandMode)
            status_.reset_command();
        mode_.reduce(a);
    });

    dispatcher.on_action<InsertAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<DeleteAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<BackspaceAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<CursorUpAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<CursorDownAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<CursorLeftAction>([this](const auto &a) { reduce(a); });
    dispatcher.on_action<CursorRightAction>([this](const auto &a) { reduce(a); });
}

const edit::BufferSlice &edit::EditorStore::buffer() const
{
    return buffer_;
}

const edit::ModeSlice &edit::EditorStore::mode() const
{
    return mode_;
}

const edit::StatusSlice &edit::EditorStore::status() const
{
    return status_;
}

void edit::EditorStore::reduce(const InsertAction &a)
{
    switch (mode_.mode())
    {
    case Mode::InsertMode:
        dispatcher_.emit(buffer_.reduce(a));
        break;
    case Mode::CommandMode:
        if (auto ev = status_.reduce(a); ev.has_value())
            dispatcher_.emit(ev.value());
        break;
    default:
        break;
    }
}

void edit::EditorStore::reduce(const DeleteAction &a)
{
    switch (mode_.mode())
    {
    case Mode::InsertMode:
        if (auto ev = buffer_.reduce(a); ev.has_value())
            dispatcher_.emit(ev.value());
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    default:
        break;
    }
}

void edit::EditorStore::reduce(const BackspaceAction &a)
{
    switch (mode_.mode())
    {
    case Mode::InsertMode:
        if (auto ev = buffer_.reduce(a); ev.has_value())
            dispatcher_.emit(ev.value());
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    default:
        break;
    }
}

void edit::EditorStore::reduce(const CursorUpAction &a)
{
    if (mode_.mode() == Mode::InsertMode)
    {
        buffer_.reduce(a);
    }
}

void edit::EditorStore::reduce(const CursorDownAction &a)
{
    if (mode_.mode() == Mode::InsertMode)
    {
        buffer_.reduce(a);
    }
}

void edit::EditorStore::reduce(const CursorLeftAction &a)
{
    switch (mode_.mode())
    {
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    default:
        break;
    }
}

void edit::EditorStore::reduce(const CursorRightAction &a)
{
    switch (mode_.mode())
    {
    case Mode::InsertMode:
        buffer_.reduce(a);
        break;
    case Mode::CommandMode:
        status_.reduce(a);
        break;
    default:
        break;
    }
}
