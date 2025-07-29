#include "ui/StatusView.hpp"
#include <sstream>

using namespace edit::ui;

StatusView::StatusView()
{
}

void StatusView::render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds)
{
    std::ostringstream stream;
    stream << "(I:" << status.get_display_cursor_index() << ",";
    stream << " Y:" << status.get_display_cursor_y() << ",";
    stream << " X:" << status.get_display_cursor_x() << ")";
    backend.put_text(bounds.anchor.y, bounds.anchor.x, stream.str());
}
