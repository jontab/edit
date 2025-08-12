#pragma once

#include "state/EditorStore.hpp"

namespace edit::ui
{

class IView
{
  public:
    virtual ~IView() = default;

    virtual void poll(edit::core::Dispatcher &dispatcher) = 0;
    virtual void render(const edit::state::EditorStore &store) = 0;
};

} // namespace edit::ui
