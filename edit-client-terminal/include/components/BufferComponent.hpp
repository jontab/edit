#pragma once

#include "ModeComponent.hpp"
#include "core/Dispatcher.hpp"
#include "core/Point.hpp"
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

    // Core.
    Dispatcher &dispatcher_;
    ModeComponent &mode_component_;

    common::Buffer buffer_;
    std::vector<Line> lines_;
    std::size_t cursor_;
    int site_;

  public:
    BufferComponent(Dispatcher &dispatcher, ModeComponent &mode_component);
    BufferComponent(Dispatcher &dispatcher, ModeComponent &mode_component, const std::vector<common::Char> &chars);
    BufferComponent(Dispatcher &dispatcher, ModeComponent &mode_component, std::vector<common::Char> &&chars);

    void set_cursor_index(std::size_t index);
    int site() const;
    std::size_t get_cursor_y() const;
    ui::Point<std::size_t> get_cursor_position() const;
    std::size_t line_count() const;
    std::size_t line_length(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_begin(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_end(std::size_t y) const;
    Mode mode() const;

  private:
    void init(Dispatcher &dispatcher);

    // Actions.
    void handle_cursor_up();
    void handle_cursor_down();
    void handle_cursor_left();
    void handle_cursor_right();
    void handle_insert(const InsertAction &action);
    void handle_delete();
    void handle_backspace();
    void handle_escape();

    void calculate_lines();
    std::size_t calculate_x(const Line &line, std::size_t index) const;
    std::size_t calculate_x_index(const Line &line, std::size_t count) const;
    std::size_t next_visible_before(std::size_t index) const;
    std::size_t next_visible_after(std::size_t index) const;
    bool is_y_at_top(std::size_t y) const;
    bool is_y_at_bottom(std::size_t y) const;
};

} // namespace edit
