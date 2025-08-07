#pragma once

#include "state/EditorStore.hpp"

namespace edit::ui
{

class IView
{
  public:
    virtual ~IView() = default;

    virtual void poll(Dispatcher &dispatcher) = 0;
    virtual void render(const EditorStore &store) = 0;
};

} // namespace edit::ui
