#pragma once

#include "core/Rect.hpp"
#include "state/EditorStore.hpp"
#include "ui/IViewBackend.hpp"
#include <ostream>

namespace edit::ui
{

class StatusView
{
    std::size_t camera_;

  public:
    StatusView();

    void render(IViewBackend &backend, const EditorStore &store, const core::Rect<unsigned int> &bounds, Mode mode);

  private:
    void render_insert_normal(IViewBackend &backend,
        const EditorStore &store,
        const core::Rect<unsigned int> &bounds,
        Mode mode);
    void render_command(IViewBackend &backend, const EditorStore &store, const core::Rect<unsigned int> &bounds);
    void adjust_camera(std::size_t cursor, const core::Rect<unsigned int> &bounds);
};

} // namespace edit::ui
