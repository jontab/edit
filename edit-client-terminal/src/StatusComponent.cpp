#include "StatusComponent.hpp"

edit::StatusComponent::StatusComponent(EventBus &event_bus)
    : display_cursor_index_(), display_cursor_y_(), display_cursor_x_()
{
    event_bus.on<CursorMoved>([this](const auto &ev) { handle_cursor_moved(ev); });
}

std::size_t edit::StatusComponent::get_display_cursor_index() const
{
    return display_cursor_index_;
}

std::size_t edit::StatusComponent::get_display_cursor_y() const
{
    return display_cursor_y_;
}

std::size_t edit::StatusComponent::get_display_cursor_x() const
{
    return display_cursor_x_;
}

void edit::StatusComponent::handle_cursor_moved(const CursorMoved &event)
{
    display_cursor_index_ = event.new_index;
    display_cursor_y_ = event.new_y;
    display_cursor_x_ = event.new_x;
}
