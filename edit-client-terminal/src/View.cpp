#define TB_IMPL
#include "View.hpp"
#include "termbox2.h"

edit::DefaultView::DefaultView() : buffer_window({0, 0, 0, 0}), status_window({0, 0, 0, 0})
{
    if (tb_init() == TB_ERR)
        throw std::runtime_error("tb_init failed");
    handle_resize();
}

edit::DefaultView::~DefaultView()
{
    tb_shutdown();
}

std::optional<edit::Action> edit::DefaultView::poll()
{
    struct tb_event ev;
    if (tb_poll_event(&ev) == TB_ERR)
        throw std::runtime_error("tb_poll_event failed");
    switch (ev.type)
    {
    case TB_EVENT_KEY:
        if (ev.ch == 'q')
            return edit::Action{edit::Quit{}};
        break;
    case TB_EVENT_RESIZE:
        handle_resize();
        break;
    default:
        break;
    }

    return std::nullopt;
}

void edit::DefaultView::render(const BufferComponent &buffer, const StatusComponent &status)
{
    render_buffer_component(buffer);
    render_status_component(status);
    if (tb_present() == TB_ERR)
        throw std::runtime_error("tb_present failed");
}

void edit::DefaultView::handle_resize()
{
    int height = tb_height();
    if (height < 0)
        throw std::runtime_error("tb_height failed");
    int width = tb_width();
    if (width < 0)
        throw std::runtime_error("tb_width failed");
    calculate_buffer_window(height, width);
    calculate_status_window(height, width);
}

void edit::DefaultView::render_buffer_component(const BufferComponent &buffer)
{
    for (int dy = 0; dy < buffer_window.h; dy++)
        for (int dx = 0; dx < buffer_window.w; dx++)
            tb_printf(buffer_window.x + dx, buffer_window.y + dy, TB_WHITE, TB_DEFAULT, "b");
}

void edit::DefaultView::render_status_component(const StatusComponent &buffer)
{
    for (int dy = 0; dy < status_window.h; dy++)
        for (int dx = 0; dx < status_window.w; dx++)
            tb_printf(status_window.x + dx, status_window.y + dy, TB_WHITE, TB_DEFAULT, "s");
}

void edit::DefaultView::calculate_buffer_window(int height, int width)
{
    buffer_window.y = 0;
    buffer_window.x = 0;
    buffer_window.h = height - 1;
    buffer_window.w = width;
}

void edit::DefaultView::calculate_status_window(int height, int width)
{
    status_window.y = height - 1;
    status_window.x = 0;
    status_window.h = 1;
    status_window.w = width;
}
