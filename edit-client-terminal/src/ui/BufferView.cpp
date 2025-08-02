#include "ui/BufferView.hpp"

using namespace edit::ui;

BufferView::BufferView()
    : camera_({0, 0})
{
}

void BufferView::render(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds)
{
    auto cursor = buffer.get_cursor_position();
    adjust_camera_y(cursor.y, bounds);
    adjust_camera_x(cursor.x, bounds);
    render_lines(backend, buffer, bounds);
    render_cursor(backend, buffer, bounds);
}

void BufferView::adjust_camera_y(std::size_t cursor_y, const Rect<unsigned int> &bounds)
{
    auto y_top = camera_.y;                    // Inclusive.
    auto y_bottom = camera_.y + bounds.size.y; // Exclusive.

    // TODO: Implement a "zone" around the cursor.
    if (cursor_y < y_top)
    {
        camera_.y = cursor_y;
    }

    // TODO: Implement a "zone" around the cursor.
    if (cursor_y >= y_bottom)
    {
        camera_.y = cursor_y - bounds.size.y + 1;
    }
}

void BufferView::adjust_camera_x(std::size_t cursor_x, const Rect<unsigned int> &bounds)
{
    auto x_left = camera_.x;                  // Inclusive.
    auto x_right = camera_.x + bounds.size.x; // Exclusive.

    // TODO: Implement a "zone" around the cursor.
    if (cursor_x < x_left)
    {
        camera_.x = cursor_x;
    }

    // TODO: Implement a "zone" around the cursor.
    if (cursor_x >= x_right)
    {
        camera_.x = cursor_x - bounds.size.x + 1;
    }
}

void BufferView::render_lines(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds)
{
    for (unsigned int view_y = 0; view_y < bounds.size.y; view_y++)
    {
        DrawLineContext ctx{
            .backend = backend,
            .bounds = bounds,
            .view_y = view_y,
            .view_x = 0,
            .view_camera_remaining = camera_.x,
        };
        for (auto it = buffer.line_begin(camera_.y + view_y); it < buffer.line_end(camera_.y + view_y); it++)
        {
            if (!it->is_deleted)
            {
                if (!render_line_callback(it->ch, ctx))
                {
                    break; // Done.
                }
            }
        }
    }
}

void BufferView::render_cursor(IViewBackend &backend, const BufferComponent &buffer, const Rect<unsigned int> &bounds)
{
    auto cursor = buffer.get_cursor_position();
    DrawCursorContext ctx{
        .bounds = bounds,
        .view_x = 0,
        .x = cursor.x,
    };
    for (auto it = buffer.line_begin(cursor.y); it < buffer.line_end(cursor.y); it++)
    {
        if (!it->is_deleted)
        {
            if (!render_cursor_callback(it->ch, ctx))
            {
                break; // Done.
            }
        }
    }

    if ((buffer.mode() == Mode::InsertMode) || (buffer.mode() == Mode::NormalMode))
    {
        // Cursor.
        auto view_y = cursor.y - camera_.y;
        auto view_x = ctx.view_x - camera_.x;
        if ((view_y < static_cast<std::size_t>(bounds.size.y)) && (view_x < static_cast<std::size_t>(bounds.size.x)))
        {
            backend.set_cursor(view_y + bounds.anchor.y, view_x + bounds.anchor.x);
        }
    }
}

bool BufferView::render_line_callback(std::uint32_t ch, DrawLineContext &ctx)
{
    if (ch == '\t')
    {
        int tabs = calculate_tab_count(ctx.view_x);
        for (int i = 0; i < tabs; i++)
            if (!render_line_callback_put(' ', ctx))
                return false;
    }
    else
    {
        if (!render_line_callback_put(ch, ctx))
            return false;
    }

    return true;
}

bool BufferView::render_line_callback_put(std::uint32_t ch, struct DrawLineContext &ctx)
{
    bool is_before = ctx.view_camera_remaining > 0;
    bool is_after = ctx.view_x >= ctx.bounds.size.x;
    if (!is_before && !is_after)
    {
        size_t view_y = ctx.view_y + ctx.bounds.anchor.y;
        size_t view_x = ctx.view_x + ctx.bounds.anchor.x;
        ctx.backend.put_char(view_y, view_x, ch);
        ctx.view_x++;
        return true; // Not done.
    }
    else if (is_before)
    {
        ctx.view_camera_remaining--;
        return true; // Not done.
    }
    else
    {
        return false; // Done.
    }
}

bool BufferView::render_cursor_callback(std::uint32_t ch, DrawCursorContext &ctx)
{
    if (ctx.x > 0)
    {
        ctx.view_x += (ch == '\t') ? calculate_tab_count(ctx.view_x) : 1;
        ctx.x--;
        return true; // Not done.
    }
    else
    {
        return false; // Done.
    }
}

int BufferView::calculate_tab_count(std::size_t view_x)
{
    return 4 - static_cast<int>(view_x % 4);
}
