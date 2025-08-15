#pragma once

#include "IViewBackend.hpp"
#include "Rect.hpp"
#include "StatusViewSnapshot.hpp"
#include "state/EditorStore.hpp"

namespace edit::ui
{

class StatusView
{
    std::size_t camera_;

  public:
    StatusView();

    void render(IViewBackend &backend, const StatusViewSnapshot &snap, const Rect<unsigned int> &bounds);

  private:
    void render_insert_normal(IViewBackend &backend, const StatusViewSnapshot &snap, const Rect<unsigned int> &bounds);
    void render_command(IViewBackend &backend, const StatusViewSnapshot &snap, const Rect<unsigned int> &bounds);
    void adjust_camera(std::size_t cursor, const Rect<unsigned int> &bounds);
};

} // namespace edit::ui
