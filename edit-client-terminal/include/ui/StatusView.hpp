#pragma once

#include "IViewComponent.hpp"
#include "components/StatusComponent.hpp"
#include <ostream>

namespace edit::ui
{

class StatusView : public IViewComponent<StatusComponent>
{
    std::size_t camera_;

  public:
    StatusView();

    void render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds);

  private:
    void render_insert_normal(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds);
    void render_command(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds);
    void adjust_camera(std::size_t cursor, const Rect<unsigned int> &bounds);
};

} // namespace edit::ui
