#pragma once

#include "../BufferComponent.hpp"
#include "../StatusComponent.hpp"

namespace edit::ui
{

class IView
{
  public:
    virtual ~IView() = default;

    virtual void poll(ActionBus &action_bus) = 0;
    virtual void render(const BufferComponent &buffer, const StatusComponent &status) = 0;
};

} // namespace edit::ui
