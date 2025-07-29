#pragma once

#include "Bus.hpp"
#include "ui/Point.hpp"
#include <edit-common/Buffer.hpp>
#include <utility>

namespace edit
{

class BufferComponent
{
    struct Line
    {
        std::size_t begin;
        std::size_t end;
    };

    EventBus &event_bus_;
    common::Buffer buffer_;
    std::vector<Line> lines_;
    std::size_t cursor_;
    int site_;

    void init(ActionBus &action_bus);

  public:
    BufferComponent(ActionBus &action_bus, EventBus &event_bus);
    BufferComponent(ActionBus &action_bus, EventBus &event_bus, const std::vector<common::Char> &chars);
    BufferComponent(ActionBus &action_bus, EventBus &event_bus, std::vector<common::Char> &&chars);

    void set_cursor_index(std::size_t index);
    int site() const;
    std::size_t get_cursor_y() const;
    ui::Point<std::size_t> get_cursor_position() const;
    std::size_t line_count() const;
    std::size_t line_length(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_begin(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_end(std::size_t y) const;

  private:
    void handle_cursor_up();
    void handle_cursor_down();
    void handle_cursor_left();
    void handle_cursor_right();
    void handle_insert(const Insert &action);
    void handle_delete();
    void handle_backspace();
    void calculate_lines();
    std::size_t calculate_x(const Line &line, std::size_t index) const;
    std::size_t calculate_x_index(const Line &line, std::size_t count) const;
    std::size_t next_visible_before(std::size_t index) const;
    std::size_t next_visible_after(std::size_t index) const;
    bool is_y_at_top(std::size_t y) const;
    bool is_y_at_bottom(std::size_t y) const;
};

} // namespace edit
