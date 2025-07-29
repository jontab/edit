#pragma once

#include "../BufferComponent.hpp"
#include "IViewComponent.hpp"

namespace edit::ui
{

class BufferView : public IViewComponent<BufferComponent>
{
    Point<std::size_t> camera_;

  public:
    BufferView();

    void render(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds);

  private:
    struct DrawLineContext
    {
        IViewBackend &backend;
        const Rect<unsigned int> &bounds;
        unsigned int view_y;
        unsigned int view_x;
        std::size_t view_camera_remaining;
    };

    struct DrawCursorContext
    {
        const Rect<unsigned int> &bounds;
        unsigned int view_x;
        std::size_t x;
    };

    void adjust_camera_y(std::size_t cursor_y, const Rect<unsigned int> &bounds);
    void adjust_camera_x(std::size_t cursor_x, const Rect<unsigned int> &bounds);
    void render_lines(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds);
    void render_cursor(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds);
    bool render_line_callback(std::uint32_t ch, DrawLineContext &ctx);
    bool render_line_callback_put(std::uint32_t ch, DrawLineContext &ctx);
    bool render_cursor_callback(std::uint32_t ch, DrawCursorContext &ctx);
    int calculate_tab_count(std::size_t view_x);
};

} // namespace edit::ui
