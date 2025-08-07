#include "state/ModeSlice.hpp"

void edit::ModeSlice::reduce(const ChangeModeAction &a)
{
    s_.mode = a.to;
}

void edit::ModeSlice::reduce(const EscapeAction &a)
{
    switch (s_.mode)
    {
    case Mode::InsertMode:
    case Mode::CommandMode:
        s_.mode = Mode::NormalMode;
        break;
    default:
        break;
    }
}

edit::Mode edit::ModeSlice::mode() const
{
    return s_.mode;
}
