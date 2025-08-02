#pragma once

#include "components/BufferComponent.hpp"
#include "components/StatusComponent.hpp"

namespace edit::ui
{

class IView
{
  public:
    virtual ~IView() = default;

    virtual void poll(Dispatcher &dispatcher) = 0;
    virtual void render(const BufferComponent &buffer, const StatusComponent &status) = 0;
};

} // namespace edit::ui
