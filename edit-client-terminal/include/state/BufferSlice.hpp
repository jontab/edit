#pragma once

#include "core/Action.hpp"
#include "core/Event.hpp"
#include "core/Point.hpp"
#include "state/BufferState.hpp"

namespace edit
{

class BufferSlice
{
    BufferState s_;

  public:
    BufferSlice();
    BufferSlice(const std::vector<common::Char> &chars);
    BufferSlice(std::vector<common::Char> &&chars);

    // Reduce.
    void reduce(const CursorUpAction &a);
    void reduce(const CursorDownAction &a);
    void reduce(const CursorLeftAction &a);
    void reduce(const CursorRightAction &a);
    CharInsertedEvent reduce(const InsertAction &a);
    std::optional<CharDeletedEvent> reduce(const DeleteAction &a);
    std::optional<CharDeletedEvent> reduce(const BackspaceAction &a);

    // Other.
    void set_cursor_index(std::size_t index);
    std::size_t cursor() const;
    int site() const;
    std::size_t get_cursor_y() const;
    core::Point<std::size_t> get_cursor_position() const;
    std::size_t line_count() const;
    std::size_t line_length(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_begin(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_end(std::size_t y) const;

  private:
    void calculate_lines();
    std::size_t calculate_x(const BufferState::Line &line, std::size_t index) const;
    std::size_t calculate_x_index(const BufferState::Line &line, std::size_t count) const;
    std::size_t next_visible_before(std::size_t index) const;
    std::size_t next_visible_after(std::size_t index) const;
    bool is_y_at_top(std::size_t y) const;
    bool is_y_at_bottom(std::size_t y) const;
};

} // namespace edit
