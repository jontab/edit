#include "state/ModeSlice.hpp"

using namespace edit::core;
using namespace edit::state;

void ModeSlice::reduce(const ChangeModeAction &a)
{
    s_.mode = a.to;
}

void ModeSlice::reduce(const EscapeAction &a)
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

Mode ModeSlice::mode() const
{
    return s_.mode;
}
