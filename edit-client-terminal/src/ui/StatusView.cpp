#include "ui/StatusView.hpp"
#include <sstream>

using namespace edit::core;
using namespace edit::ui;

StatusView::StatusView()
    : camera_(0)
{
}

void StatusView::render(IViewBackend &backend, const StatusViewSnapshot &snap, const Rect<unsigned int> &bounds)
{
    switch (snap.current_mode)
    {
    case Mode::CommandMode:
        render_command(backend, snap, bounds);
        break;
    default:
        render_insert_normal(backend, snap, bounds);
        break;
    }
}

void StatusView::render_insert_normal(IViewBackend &backend,
    const StatusViewSnapshot &snap,
    const Rect<unsigned int> &bounds)
{
    std::ostringstream stream;

    // Mode.
    switch (snap.current_mode)
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
    stream << "(I:" << snap.buffer_cursor_index << ",";
    stream << " Y:" << snap.buffer_cursor_y << ",";
    stream << " X:" << snap.buffer_cursor_x << ")";

    // Status.
    if (!snap.slice.state().status.empty())
    {
        stream << " | ";
        stream << snap.slice.state().status;
    }

    backend.put_text(bounds.anchor.y, bounds.anchor.x, stream.str());
}

void StatusView::render_command(IViewBackend &backend, const StatusViewSnapshot &snap, const Rect<unsigned int> &bounds)
{
    adjust_camera(snap.slice.state().command_cursor, bounds);

    // Content.
    auto show = snap.slice.state().command_content.substr(camera_);
    backend.put_text(bounds.anchor.y, bounds.anchor.x, show);

    // Cursor.
    auto view_x = snap.slice.state().command_cursor - camera_;
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
