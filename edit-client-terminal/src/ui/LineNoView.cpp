#include "ui/LineNoView.hpp"

edit::ui::LineNoView::render(IViewBackend &backend, std::size_t camera_y, const Rect<unsigned int> &bounds)
{
    if (bounds.size.x == 0)
    {
        return;
    }

    for (unsigned int dy = 0; dy < bounds.size.y; dy++)
    {
        auto line_i = camera_y + dy;
        auto view_y = bounds.anchor.y + dy;
        backend.put_text(view_y, bounds.anchor.x, std::to_string(line_i));
    }
}
