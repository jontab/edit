#pragma once

#include "IViewBackend.hpp"
#include "core/Rect.hpp"
#include "state/BufferSlice.hpp"

namespace edit::ui
{

class BufferView
{

    core::Point<std::size_t> camera_;

  public:
    BufferView();

    void render(IViewBackend &backend, const BufferSlice &buffer, const core::Rect<unsigned int> &bounds, Mode mode);

  private:
    struct DrawLineContext
    {
        IViewBackend &backend;
        const core::Rect<unsigned int> &bounds;
        unsigned int view_y;
        unsigned int view_x;
        std::size_t view_camera_remaining;
    };

    struct DrawCursorContext
    {
        const core::Rect<unsigned int> &bounds;
        unsigned int view_x;
        std::size_t x;
    };

    void adjust_camera_y(std::size_t cursor_y, const core::Rect<unsigned int> &bounds);
    void adjust_camera_x(std::size_t cursor_x, const core::Rect<unsigned int> &bounds);
    void render_lines(IViewBackend &backend, const BufferSlice &buffer, const core::Rect<unsigned int> &bounds);
    void render_cursor(IViewBackend &backend,
        const BufferSlice &buffer,
        const core::Rect<unsigned int> &bounds,
        Mode mode);
    bool render_line_callback(std::uint32_t ch, DrawLineContext &ctx);
    bool render_line_callback_put(std::uint32_t ch, DrawLineContext &ctx);
    bool render_cursor_callback(std::uint32_t ch, DrawCursorContext &ctx);
    int calculate_tab_count(std::size_t view_x);
};

} // namespace edit::ui
