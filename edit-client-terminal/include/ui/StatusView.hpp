#pragma once

#include "../StatusComponent.hpp"
#include "IViewComponent.hpp"

namespace edit::ui
{

class StatusView : public IViewComponent<StatusComponent>
{
  public:
    StatusView();

    void render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds);
};

} // namespace edit::ui
