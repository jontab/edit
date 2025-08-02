#include "ui/StatusView.hpp"
#include <sstream>

using namespace edit::ui;

StatusView::StatusView()
{
}

void StatusView::render(IViewBackend &backend, const StatusComponent &status, const Rect<unsigned int> &bounds)
{
    std::ostringstream stream;

    // Cursor.
    stream << "(I:" << status.display_cursor_index() << ",";
    stream << " Y:" << status.display_cursor_y() << ",";
    stream << " X:" << status.display_cursor_x() << ")";

    // Status.
    if (!status.status().empty())
    {
        stream << " | " << status.status();
    }

    backend.put_text(bounds.anchor.y, bounds.anchor.x, stream.str());
}
