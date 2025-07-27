#pragma once

#include "Bus.hpp"

namespace edit
{

class StatusComponent
{
    int display_cursor_index;
    int display_cursor_y;
    int display_cursor_x;

  public:
    StatusComponent(EventBus &event_bus);

    int get_display_cursor_y();
    int get_display_cursor_x();
    int get_display_cursor_index();

  private:
    void handle_cursor_moved(const CursorMoved &event);
};

} // namespace edit
