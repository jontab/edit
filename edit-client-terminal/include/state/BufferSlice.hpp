#pragma once

#include "core/Action.hpp"
#include "core/Event.hpp"
#include "core/Point.hpp"
#include "state/BufferState.hpp"

namespace edit::state
{

class BufferSlice
{
    BufferState s_;

  public:
    BufferSlice();
    BufferSlice(const std::vector<common::Char> &chars);
    BufferSlice(std::vector<common::Char> &&chars);

    const BufferState &state() const;

    // Reduce.
    void reduce(const edit::core::CursorUpAction &a);
    void reduce(const edit::core::CursorDownAction &a);
    void reduce(const edit::core::CursorLeftAction &a);
    void reduce(const edit::core::CursorRightAction &a);
    edit::core::CharInsertedEvent reduce(const edit::core::InsertAction &a);
    std::optional<edit::core::CharDeletedEvent> reduce(const edit::core::DeleteAction &a);
    std::optional<edit::core::CharDeletedEvent> reduce(const edit::core::BackspaceAction &a);

    // Other.
    void set_cursor_index(std::size_t index);
    std::size_t cursor() const;
    int site() const;
    std::size_t get_cursor_y() const;
    edit::core::Point<std::size_t> get_cursor_position() const;
    std::size_t line_count() const;
    std::size_t line_length(std::size_t y) const;

  private:
    void calculate_lines();
    std::size_t calculate_x(const BufferState::Line &line, std::size_t index) const;
    std::size_t calculate_x_index(const BufferState::Line &line, std::size_t count) const;
    std::size_t next_visible_before(std::size_t index) const;
    std::size_t next_visible_after(std::size_t index) const;
    bool is_y_at_top(std::size_t y) const;
    bool is_y_at_bottom(std::size_t y) const;
};

} // namespace edit::state
