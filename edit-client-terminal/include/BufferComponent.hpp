#pragma once

#include "Bus.hpp"
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

    common::Buffer buffer_;
    std::vector<Line> lines_;
    std::size_t cursor_;
    int site_;

    void init(ActionBus &action_bus);

  public:
    BufferComponent(ActionBus &action_bus);
    BufferComponent(ActionBus &action_bus, const std::vector<common::Char> &chars);
    BufferComponent(ActionBus &action_bus, std::vector<common::Char> &&chars);

    int site() const;
    void set_cursor_index(std::size_t index);
    std::size_t get_cursor_index() const;
    std::size_t get_cursor_y() const;
    std::pair<std::size_t, std::size_t> get_cursor_position() const;

  private:
    void handle_cursor_up();
    void handle_cursor_down();
    void handle_cursor_left();
    void handle_cursor_right();

    void calculate_lines();
    std::size_t calculate_line_offset(const Line &line, std::size_t index) const;
    std::size_t calculate_line_offset_add(const Line &line, std::size_t count) const;
};

} // namespace edit
