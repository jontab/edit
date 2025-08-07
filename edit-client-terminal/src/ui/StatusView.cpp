#include "ui/StatusView.hpp"
#include <sstream>

using namespace edit::ui;

StatusView::StatusView()
    : camera_(0)
{
}

void StatusView::render(IViewBackend &backend,
    const EditorStore &store,
    const core::Rect<unsigned int> &bounds,
    Mode mode)
{
    switch (mode)
    {
    case Mode::CommandMode:
        render_command(backend, store, bounds);
        break;
    default:
        render_insert_normal(backend, store, bounds, mode);
        break;
    }
}

void StatusView::render_insert_normal(IViewBackend &backend,
    const EditorStore &store,
    const core::Rect<unsigned int> &bounds,
    Mode mode)
{
    std::ostringstream stream;

    // Mode.
    switch (mode)
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
    auto pos = store.buffer().get_cursor_position();
    stream << " | ";
    stream << "(I:" << store.buffer().cursor() << ",";
    stream << " Y:" << pos.y << ",";
    stream << " X:" << pos.x << ")";

    // Status.
    if (!store.status().status().empty())
    {
        stream << " | ";
        stream << store.status().status();
    }

    backend.put_text(bounds.anchor.y, bounds.anchor.x, stream.str());
}

void StatusView::render_command(IViewBackend &backend, const EditorStore &store, const core::Rect<unsigned int> &bounds)
{
    adjust_camera(store.status().command_cursor(), bounds);

    // Content.
    auto show = store.status().command_content().substr(camera_);
    backend.put_text(bounds.anchor.y, bounds.anchor.x, show);

    // Cursor.
    auto view_x = store.status().command_cursor() - camera_;
    backend.set_cursor(bounds.anchor.y, bounds.anchor.x + view_x);
}

void StatusView::adjust_camera(std::size_t cursor, const core::Rect<unsigned int> &bounds)
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
