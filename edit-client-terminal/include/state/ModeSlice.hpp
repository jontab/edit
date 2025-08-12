#pragma once

#include "core/Action.hpp"
#include "state/ModeState.hpp"

namespace edit::state
{

class ModeSlice
{
    ModeState s_;

  public:
    // Reduce.
    void reduce(const edit::core::ChangeModeAction &a);
    void reduce(const edit::core::EscapeAction &a);

    // Other.
    edit::core::Mode mode() const;
};

} // namespace edit::state
