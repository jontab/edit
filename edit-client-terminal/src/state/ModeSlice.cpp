#include "state/ModeSlice.hpp"

using namespace edit::core;
using namespace edit::state;

void ModeSlice::reduce(const actions::ChangeMode &a)
{
    s_.mode = a.to;
}

void ModeSlice::reduce(const actions::Escape &a)
{
    switch (s_.mode)
    {
    case Mode::NormalMode:
        break;
    case Mode::InsertMode:
    case Mode::CommandMode:
        s_.mode = Mode::NormalMode;
        break;
    }
}

Mode ModeSlice::mode() const
{
    return s_.mode;
}
