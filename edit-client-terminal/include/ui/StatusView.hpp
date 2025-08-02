#pragma once

#include "IViewComponent.hpp"
#include "components/StatusComponent.hpp"

namespace edit::ui
{

class StatusView : public IViewComponent<StatusComponent>
{
  public:
    StatusView();

    void render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds);
};

} // namespace edit::ui
