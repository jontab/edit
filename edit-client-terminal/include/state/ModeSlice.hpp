#pragma once

#include "core/Action.hpp"
#include "state/ModeState.hpp"

namespace edit
{

class ModeSlice
{
  public:
    // Reduce.
    void reduce(const ChangeModeAction &a);
    void reduce(const EscapeAction &a);

    // Other.
    Mode mode() const;

  private:
    ModeState s_;
};

} // namespace edit
