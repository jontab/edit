#include "StatusComponent.hpp"

edit::StatusComponent::StatusComponent(EventBus &event_bus)
    : display_cursor_index(), display_cursor_y(), display_cursor_x()
{
    event_bus.on<CursorMoved>([this](const auto &ev) { handle_cursor_moved(ev); });
}

int edit::StatusComponent::get_display_cursor_index()
{
    return display_cursor_index;
}

int edit::StatusComponent::get_display_cursor_y()
{
    return display_cursor_y;
}

int edit::StatusComponent::get_display_cursor_x()
{
    return display_cursor_x;
}

void edit::StatusComponent::handle_cursor_moved(const CursorMoved &event)
{
    display_cursor_index = event.new_index;
    display_cursor_y = event.new_y;
    display_cursor_x = event.new_x;
}
