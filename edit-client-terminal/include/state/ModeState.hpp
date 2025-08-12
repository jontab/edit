#pragma once

#include "core/Mode.hpp"

namespace edit::state
{

struct ModeState
{
    edit::core::Mode mode = edit::core::Mode::NormalMode;
};

} // namespace edit
