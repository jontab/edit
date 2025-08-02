#pragma once

#include "ModeComponent.hpp"
#include "core/Bus.hpp"

namespace edit
{

class StatusComponent
{
    // Core.
    ActionBus &action_bus_;
    ModeComponent &mode_component_;

    // State.
    std::size_t display_cursor_index_;
    std::size_t display_cursor_y_;
    std::size_t display_cursor_x_;
    std::string status_;
    std::size_t command_cursor_;
    std::string command_content_;

  public:
    StatusComponent(ActionBus &action_bus, EventBus &event_bus, ModeComponent &mode_component);

    std::size_t display_cursor_y() const;
    std::size_t display_cursor_x() const;
    std::size_t display_cursor_index() const;
    const std::string &status() const;
    std::size_t command_cursor() const;
    const std::string &command_content() const;
    Mode mode() const;

  private:
    // Events.
    void handle_cursor_moved(const CursorMovedEvent &ev);
    void handle_mode_changed(const ModeChangedEvent &ev);

    // Actions.
    void handle_escape();
    void handle_cursor_left();
    void handle_cursor_right();
    void handle_insert(const InsertAction &ev);
    void handle_delete();
    void handle_backspace();
};

} // namespace edit
