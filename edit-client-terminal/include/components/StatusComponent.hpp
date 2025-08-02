#pragma once

#include "core/Bus.hpp"

namespace edit
{

class StatusComponent
{
    int display_cursor_index_;
    int display_cursor_y_;
    int display_cursor_x_;
    std::string status_;

  public:
    StatusComponent(ActionBus &action_bus, EventBus &event_bus);

    std::size_t display_cursor_y() const;
    std::size_t display_cursor_x() const;
    std::size_t display_cursor_index() const;
    const std::string &status() const;

  private:
    void handle_cursor_moved(const CursorMoved &event);
};

} // namespace edit
