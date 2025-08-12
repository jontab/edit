#include "state/BufferSlice.hpp"

using namespace edit::core;
using namespace edit::state;

BufferSlice::BufferSlice()
    : s_()
{
    calculate_lines();
}

BufferSlice::BufferSlice(const std::vector<common::Char> &chars)
    : s_(chars)
{
    calculate_lines();
}

BufferSlice::BufferSlice(std::vector<common::Char> &&chars)
    : s_(std::move(chars))
{
    calculate_lines();
}

const BufferState &BufferSlice::state() const
{
    return s_;
}

/******************************************************************************/
/* Reduce                                                                     */
/******************************************************************************/

void BufferSlice::reduce(const CursorUpAction &a)
{
    auto y = get_cursor_y();
    if (!is_y_at_top(y))
    {
        auto x = calculate_x(s_.lines[y], s_.cursor);
        s_.cursor = calculate_x_index(s_.lines[y - 1], x);
    }
}

void BufferSlice::reduce(const CursorDownAction &a)
{
    auto y = get_cursor_y();
    if (!is_y_at_bottom(y))
    {
        auto x = calculate_x(s_.lines[y], s_.cursor);
        s_.cursor = calculate_x_index(s_.lines[y + 1], x);
    }
}

void BufferSlice::reduce(const CursorLeftAction &a)
{
    s_.cursor = next_visible_before(s_.cursor);
}

void BufferSlice::reduce(const CursorRightAction &a)
{
    if (s_.cursor >= s_.buffer.size())
    {
        return;
    }

    if (!s_.buffer[s_.cursor].is_deleted)
    {
        // If our cursor is on a visible character, it is hovering over that character. Move to the next visible
        // character on the right.
        s_.cursor = next_visible_after(s_.cursor);
    }
    else
    {
        // If our cursor is on a deleted character, it is already hovering over the next visible character. This means
        // that we need to skip the first visible character we see on the right.
        s_.cursor = next_visible_after(s_.cursor);
        s_.cursor = next_visible_after(s_.cursor);
    }
}

CharInsertedEvent BufferSlice::reduce(const InsertAction &a)
{
    int parent_clock = 0;
    int parent_site = -1;
    if (s_.cursor > 0)
    {
        parent_clock = s_.buffer[s_.cursor - 1].clock;
        parent_site = s_.buffer[s_.cursor - 1].site;
    }

    common::Char ch{
        .parent_clock = parent_clock,
        .parent_site = parent_site,
        .clock = s_.buffer.clock() + 1,
        .site = s_.site,
        .ch = a.ch,
        .is_deleted = false,
    };
    s_.buffer.insert(ch);
    calculate_lines();
    return CharInsertedEvent{ch};
}

std::optional<CharDeletedEvent> BufferSlice::reduce(const DeleteAction &a)
{
    if (s_.cursor >= s_.buffer.size())
    {
        return std::nullopt;
    }

    if (s_.buffer[s_.cursor].is_deleted)
    {
        if (auto i = next_visible_after(s_.cursor); i < s_.buffer.size())
        {
            s_.buffer[i].is_deleted = true;
            calculate_lines();
            return CharDeletedEvent{s_.buffer[i]};
        }
    }
    else
    {
        s_.buffer[s_.cursor].is_deleted = true;
        calculate_lines();
        return CharDeletedEvent{s_.buffer[s_.cursor]};
    }

    return std::nullopt;
}

std::optional<CharDeletedEvent> BufferSlice::reduce(const BackspaceAction &a)
{
    if (s_.cursor > 0)
    {
        reduce(CursorLeftAction{});
        return reduce(DeleteAction{});
    }
    else
    {
        return std::nullopt;
    }
}

/******************************************************************************/
/* Other                                                                      */
/******************************************************************************/

void BufferSlice::set_cursor_index(std::size_t index)
{
    s_.cursor = std::min(index, s_.buffer.size());
}

std::size_t BufferSlice::cursor() const
{
    return s_.cursor;
}

int BufferSlice::site() const
{
    return s_.site;
}

std::size_t BufferSlice::get_cursor_y() const
{
    std::size_t l = 0;
    std::size_t r = s_.lines.size() - 1;
    while (l <= r)
    {
        std::size_t m = (l + r) / 2;
        if (s_.cursor < s_.lines[m].begin)
        {
            r = m - 1;
        }
        else if (s_.cursor > s_.lines[m].end)
        {
            l = m + 1;
        }
        else
        {
            return m;
        }
    }

    throw std::range_error("Cursor does not fall into valid line");
}

/**
 * @brief Return cursor position as a coordinate.
 * @return Coordinate.
 */
Point<std::size_t> BufferSlice::get_cursor_position() const
{
    auto y = get_cursor_y();
    auto x = calculate_x(s_.lines[y], s_.cursor);
    return Point<std::size_t>{y, x};
}

std::size_t BufferSlice::line_count() const
{
    return s_.lines.size();
}

std::size_t BufferSlice::line_length(std::size_t y) const
{
    if (y < s_.lines.size())
    {
        return s_.lines[y].end - s_.lines[y].begin;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Re-index the lines in this buffer.
 *
 * 1. For cursor membership questions (what line am I in?), `line.end` is inclusive.
 * 2. For content questions (what do I see?), `line.end` is exclusive of the '\\n' or `EOF`.
 *
 * These rules allow the cursor to hover over the '\\n' or `EOF`. The lines in the buffer are disjoint.
 */
void BufferSlice::calculate_lines()
{
    s_.lines.clear();
    std::size_t begin = 0;
    for (auto it = s_.buffer.begin(); it < s_.buffer.end(); it++)
    {
        if (!it->is_deleted && (it->ch == '\n'))
        {
            auto i = static_cast<std::size_t>(it - s_.buffer.begin());
            s_.lines.push_back(BufferState::Line{begin, i});
            begin = i + 1;
        }
    }

    s_.lines.push_back(BufferState::Line{begin, s_.buffer.size()});
}

/**
 * @brief Return what `nth` visible character this is in the given line.
 * @param line Line.
 * @param index Character.
 * @return What `nth` visible character this is.
 */
std::size_t BufferSlice::calculate_x(const BufferState::Line &line, std::size_t index) const
{
    auto range_begin = s_.buffer.begin() + line.begin;
    auto range_end = s_.buffer.begin() + index;
    return std::count_if(range_begin, range_end, [](const common::Char &ch) { return !ch.is_deleted; });
}

/**
 * @brief Returns index of `nth` visible character in line, or `line.end` if `count` exceeds visible character count.
 * @param line Line.
 * @param count `nth` visible character.
 * @return Index into buffer.
 */
std::size_t BufferSlice::calculate_x_index(const BufferState::Line &line, std::size_t count) const
{
    for (std::size_t i = line.begin; i < line.end; i++)
        if (!s_.buffer[i].is_deleted)
            if ((count--) == 0)
                return i;
    return line.end;
}

/**
 * @brief Find the index of the next visible character, before an index.
 * @param index Index.
 * @return Next index, or 0.
 */
std::size_t BufferSlice::next_visible_before(std::size_t index) const
{
    if (index > 0)
        for (auto i = index; (i--) > 0;)
            if (!s_.buffer[i].is_deleted)
                return i;
    return 0;
}

/**
 * @brief Find the index of the next visible character, after an index.
 * @param index Index.
 * @return Next index, or `buffer_.size()`.
 */
std::size_t BufferSlice::next_visible_after(std::size_t index) const
{
    for (auto i = index + 1; i < s_.buffer.size(); i++)
        if (!s_.buffer[i].is_deleted)
            return i;
    return s_.buffer.size();
}

bool BufferSlice::is_y_at_top(std::size_t y) const
{
    return y == 0;
}

bool BufferSlice::is_y_at_bottom(std::size_t y) const
{
    return y == s_.lines.size() - 1;
}
