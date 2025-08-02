#include "ui/StatusView.hpp"
#include <sstream>

using namespace edit::ui;

StatusView::StatusView()
    : camera_(0)
{
}

void StatusView::render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds)
{
    switch (status.mode())
    {
    case Mode::CommandMode:
        render_command(backend, status, bounds);
        break;
    default:
        render_insert_normal(backend, status, bounds);
        break;
    }
}

void StatusView::render_insert_normal(IViewBackend &backend,
    const StatusComponent &status,
    const Rect<unsigned int> &bounds)
{
    std::ostringstream stream;

    // Mode.
    switch (status.mode())
    {
    case Mode::NormalMode:
        stream << "Normal";
        break;
    case Mode::InsertMode:
        stream << "Insert";
        break;
    default:
        break;
    }

    // Cursor.
    stream << " | ";
    stream << "(I:" << status.display_cursor_index() << ",";
    stream << " Y:" << status.display_cursor_y() << ",";
    stream << " X:" << status.display_cursor_x() << ")";

    // Status.
    if (!status.status().empty())
    {
        stream << " | ";
        stream << status.status();
    }

    backend.put_text(bounds.anchor.y, bounds.anchor.x, stream.str());
}

void StatusView::render_command(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds)
{
    adjust_camera(status.command_cursor(), bounds);

    // Content.
    auto show = status.command_content().substr(camera_);
    backend.put_text(bounds.anchor.y, bounds.anchor.x, show);

    // Cursor.
    auto view_x = status.command_cursor() - camera_;
    backend.set_cursor(bounds.anchor.y, bounds.anchor.x + view_x);
}

void StatusView::adjust_camera(std::size_t cursor, const Rect<unsigned int> &bounds)
{
    auto x_left = camera_;                  // Inclusive.
    auto x_right = camera_ + bounds.size.x; // Exclusive.

    // TODO: Implement a "zone" around the cursor.
    if (cursor < x_left)
    {
        camera_ = cursor;
    }

    // TODO: Implement a "zone" around the cursor.
    if (cursor >= x_right)
    {
        camera_ = cursor - bounds.size.x + 1;
    }
}
