#pragma once

#include "core/ActionTypes.hpp"
#include "state/ModeState.hpp"

namespace edit::state
{

class ModeSlice
{
    ModeState s_;

  public:
    // Reduce.
    void reduce(const core::actions::ChangeMode &a);
    void reduce(const core::actions::Escape &a);

    // Other.
    core::Mode mode() const;
};

} // namespace edit::state
