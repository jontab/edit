#include "BufferComponent.hpp"

edit::BufferComponent::BufferComponent(ActionBus &action_bus)
{
    action_bus.on<CursorUp>([this](const auto &) { handle_cursor_up(); });
    action_bus.on<CursorDown>([this](const auto &) { handle_cursor_down(); });
    action_bus.on<CursorLeft>([this](const auto &) { handle_cursor_left(); });
    action_bus.on<CursorRight>([this](const auto &) { handle_cursor_right(); });
}

int edit::BufferComponent::get_camera_y()
{
    return 0;
}

int edit::BufferComponent::get_camera_x()
{
    return 0;
}

void edit::BufferComponent::handle_cursor_up()
{
}

void edit::BufferComponent::handle_cursor_down()
{
}

void edit::BufferComponent::handle_cursor_left()
{
}

void edit::BufferComponent::handle_cursor_right()
{
}
