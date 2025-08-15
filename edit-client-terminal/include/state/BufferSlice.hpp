#pragma once

#include "core/ActionTypes.hpp"
#include "core/EventTypes.hpp"
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

    void reduce(const core::actions::CursorUp &a);
    void reduce(const core::actions::CursorDown &a);
    void reduce(const core::actions::CursorLeft &a);
    void reduce(const core::actions::CursorRight &a);
    void reduce(const core::actions::RemoteInsert &a); // TODO: Maybe emit?
    void reduce(const core::actions::RemoteDelete &a); // TODO: Maybe emit?

    core::events::CharInserted reduce(const core::actions::Insert &a);
    std::optional<core::events::CharDeleted> reduce(const core::actions::Delete &a);
    std::optional<core::events::CharDeleted> reduce(const core::actions::Backspace &a);

    void set_cursor_index(std::size_t index);
    std::size_t cursor() const;
    int site() const;
    std::size_t get_cursor_y() const;
    core::Point<std::size_t> get_cursor_position() const;
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
