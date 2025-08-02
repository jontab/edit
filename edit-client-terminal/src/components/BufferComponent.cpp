#include "components/BufferComponent.hpp"

edit::BufferComponent::BufferComponent(ActionBus &action_bus, EventBus &event_bus)
    : BufferComponent(action_bus, event_bus, {})
{
}

edit::BufferComponent::BufferComponent(ActionBus &action_bus,
    EventBus &event_bus,
    const std::vector<common::Char> &chars)
    : event_bus_(event_bus)
    , buffer_(chars)
    , lines_()
    , cursor_(0)
    , site_(rand())
{
    init(action_bus);
}

edit::BufferComponent::BufferComponent(ActionBus &action_bus, EventBus &event_bus, std::vector<common::Char> &&chars)
    : event_bus_(event_bus)
    , buffer_(std::move(chars))
    , lines_()
    , cursor_(0)
    , site_(rand())
{
    init(action_bus);
}

void edit::BufferComponent::init(ActionBus &action_bus)
{
    action_bus.on<CursorUp>([this](const auto &) { handle_cursor_up(); });
    action_bus.on<CursorDown>([this](const auto &) { handle_cursor_down(); });
    action_bus.on<CursorLeft>([this](const auto &) { handle_cursor_left(); });
    action_bus.on<CursorRight>([this](const auto &) { handle_cursor_right(); });
    action_bus.on<Insert>([this](const auto &ev) { handle_insert(ev); });
    action_bus.on<Delete>([this](const auto &) { handle_delete(); });
    action_bus.on<Backspace>([this](const auto &) { handle_backspace(); });
    calculate_lines();
}

void edit::BufferComponent::set_cursor_index(std::size_t index)
{
    cursor_ = std::min(index, buffer_.size());
}

int edit::BufferComponent::site() const
{
    return site_;
}

std::size_t edit::BufferComponent::get_cursor_y() const
{
    std::size_t l = 0;
    std::size_t r = lines_.size() - 1;
    while (l <= r)
    {
        std::size_t m = (l + r) / 2;
        if (cursor_ < lines_[m].begin)
        {
            r = m - 1;
        }
        else if (cursor_ > lines_[m].end)
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
edit::ui::Point<std::size_t> edit::BufferComponent::get_cursor_position() const
{
    auto y = get_cursor_y();
    auto x = calculate_x(lines_[y], cursor_);
    return edit::ui::Point<std::size_t>{y, x};
}

std::size_t edit::BufferComponent::line_count() const
{
    return lines_.size();
}

std::size_t edit::BufferComponent::line_length(std::size_t y) const
{
    if (y < lines_.size())
    {
        return lines_[y].end - lines_[y].begin;
    }
    else
    {
        return 0;
    }
}

std::vector<edit::common::Char>::const_iterator edit::BufferComponent::line_begin(std::size_t y) const
{
    if (y < lines_.size())
    {
        return buffer_.begin() + lines_[y].begin;
    }
    else
    {
        return buffer_.end();
    }
}

std::vector<edit::common::Char>::const_iterator edit::BufferComponent::line_end(std::size_t y) const
{
    if (y < lines_.size())
    {
        return buffer_.begin() + lines_[y].end;
    }
    else
    {
        return buffer_.end();
    }
}

void edit::BufferComponent::handle_cursor_up()
{
    auto y = get_cursor_y();
    if (!is_y_at_top(y))
    {
        auto x = calculate_x(lines_[y], cursor_);
        cursor_ = calculate_x_index(lines_[y - 1], x);
    }

    event_bus_.publish(edit::CursorMoved{
        .new_index = cursor_,
        .new_y = get_cursor_position().y,
        .new_x = get_cursor_position().x,
    });
}

void edit::BufferComponent::handle_cursor_down()
{
    auto y = get_cursor_y();
    if (!is_y_at_bottom(y))
    {
        auto x = calculate_x(lines_[y], cursor_);
        cursor_ = calculate_x_index(lines_[y + 1], x);
    }

    event_bus_.publish(edit::CursorMoved{
        .new_index = cursor_,
        .new_y = get_cursor_position().y,
        .new_x = get_cursor_position().x,
    });
}

void edit::BufferComponent::handle_cursor_left()
{
    cursor_ = next_visible_before(cursor_);

    event_bus_.publish(edit::CursorMoved{
        .new_index = cursor_,
        .new_y = get_cursor_position().y,
        .new_x = get_cursor_position().x,
    });
}

void edit::BufferComponent::handle_cursor_right()
{
    if (cursor_ >= buffer_.size())
    {
        return;
    }

    if (!buffer_[cursor_].is_deleted)
    {
        // If our cursor is on a visible character, it is hovering over that character. Move to the next visible
        // character on the right.
        cursor_ = next_visible_after(cursor_);
    }
    else
    {
        // If our cursor is on a deleted character, it is already hovering over the next visible character. This means
        // that we need to skip the first visible character we see on the right.
        cursor_ = next_visible_after(cursor_);
        cursor_ = next_visible_after(cursor_);
    }

    event_bus_.publish(edit::CursorMoved{
        .new_index = cursor_,
        .new_y = get_cursor_position().y,
        .new_x = get_cursor_position().x,
    });
}

void edit::BufferComponent::handle_insert(const Insert &action)
{
    int parent_clock = 0;
    int parent_site = -1;
    if (cursor_ > 0)
    {
        parent_clock = buffer_[cursor_ - 1].clock;
        parent_site = buffer_[cursor_ - 1].site;
    }

    edit::common::Char ch{
        .parent_clock = parent_clock,
        .parent_site = parent_site,
        .clock = buffer_.clock() + 1,
        .site = site_,
        .ch = action.ch,
        .is_deleted = false,
    };
    buffer_.insert(ch);
    calculate_lines();
    event_bus_.publish(CharInserted{ch});
}

void edit::BufferComponent::handle_delete()
{
    if (cursor_ >= buffer_.size())
    {
        return;
    }

    if (buffer_[cursor_].is_deleted)
    {
        if (auto i = next_visible_after(cursor_); i < buffer_.size())
        {
            buffer_[i].is_deleted = true;
            calculate_lines();
        }
    }
    else
    {
        buffer_[cursor_].is_deleted = true;
        calculate_lines();
    }
}

void edit::BufferComponent::handle_backspace()
{
    if (cursor_ > 0)
    {
        handle_cursor_left();
        handle_delete();
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
void edit::BufferComponent::calculate_lines()
{
    lines_.clear();
    std::size_t begin = 0;
    for (auto it = buffer_.begin(); it < buffer_.end(); it++)
    {
        if (!it->is_deleted && (it->ch == '\n'))
        {
            auto i = static_cast<std::size_t>(it - buffer_.begin());
            lines_.push_back(Line{begin, i});
            begin = i + 1;
        }
    }

    lines_.push_back(Line{begin, buffer_.size()});
}

/**
 * @brief Return what `nth` visible character this is in the given line.
 * @param line Line.
 * @param index Character.
 * @return What `nth` visible character this is.
 */
std::size_t edit::BufferComponent::calculate_x(const Line &line, std::size_t index) const
{
    auto range_begin = buffer_.begin() + line.begin;
    auto range_end = buffer_.begin() + index;
    return std::count_if(range_begin, range_end, [](const common::Char &ch) { return !ch.is_deleted; });
}

/**
 * @brief Returns index of `nth` visible character in line, or `line.end` if `count` exceeds visible character count.
 * @param line Line.
 * @param count `nth` visible character.
 * @return Index into buffer.
 */
std::size_t edit::BufferComponent::calculate_x_index(const Line &line, std::size_t count) const
{
    for (std::size_t i = line.begin; i < line.end; i++)
        if (!buffer_[i].is_deleted)
            if ((count--) == 0)
                return i;
    return line.end;
}

/**
 * @brief Find the index of the next visible character, before an index.
 * @param index Index.
 * @return Next index, or 0.
 */
std::size_t edit::BufferComponent::next_visible_before(std::size_t index) const
{
    if (index > 0)
        for (auto i = index; (i--) > 0;)
            if (!buffer_[i].is_deleted)
                return i;
    return 0;
}

/**
 * @brief Find the index of the next visible character, after an index.
 * @param index Index.
 * @return Next index, or `buffer_.size()`.
 */
std::size_t edit::BufferComponent::next_visible_after(std::size_t index) const
{
    for (auto i = index + 1; i < buffer_.size(); i++)
        if (!buffer_[i].is_deleted)
            return i;
    return buffer_.size();
}

bool edit::BufferComponent::is_y_at_top(std::size_t y) const
{
    return y == 0;
}

bool edit::BufferComponent::is_y_at_bottom(std::size_t y) const
{
    return y == lines_.size() - 1;
}
