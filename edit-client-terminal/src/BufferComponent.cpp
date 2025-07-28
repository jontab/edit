#include "BufferComponent.hpp"

edit::BufferComponent::BufferComponent(ActionBus &action_bus) : BufferComponent(action_bus, {})
{
}

edit::BufferComponent::BufferComponent(ActionBus &action_bus, const std::vector<common::Char> &chars)
    : buffer_(chars), lines_(), cursor_(0), site_(rand())
{
    init(action_bus);
}

edit::BufferComponent::BufferComponent(ActionBus &action_bus, std::vector<common::Char> &&chars)
    : buffer_(std::move(chars)), lines_(), cursor_(0), site_(rand())
{
    init(action_bus);
}

void edit::BufferComponent::init(ActionBus &action_bus)
{
    action_bus.on<CursorUp>([this](const auto &) { handle_cursor_up(); });
    action_bus.on<CursorDown>([this](const auto &) { handle_cursor_down(); });
    action_bus.on<CursorLeft>([this](const auto &) { handle_cursor_left(); });
    action_bus.on<CursorRight>([this](const auto &) { handle_cursor_right(); });
    calculate_lines();
}

int edit::BufferComponent::site() const
{
    return site_;
}

void edit::BufferComponent::set_cursor_index(std::size_t index)
{
    cursor_ = std::min(index, buffer_.size());
}

std::size_t edit::BufferComponent::get_cursor_index() const
{
    return cursor_;
}

std::size_t edit::BufferComponent::get_cursor_y() const
{
    for (auto it = lines_.begin(); it < lines_.end(); it++)
        if ((it->begin <= cursor_) && (cursor_ <= it->end))
            return it - lines_.begin();
    throw std::range_error("Cursor does not fall into valid line");
}

/**
 * @brief Return cursor position as a (y, x)-coordinate.
 * @return (y, x)-coordinate.
 */
std::pair<std::size_t, std::size_t> edit::BufferComponent::get_cursor_position() const
{
    auto y = get_cursor_y();
    auto x = calculate_line_offset(lines_[y], cursor_);
    return std::make_pair(y, x);
}

void edit::BufferComponent::handle_cursor_up()
{
    auto y = get_cursor_y();
    if (y > 0)
    {
        auto x = calculate_line_offset(lines_[y], cursor_);
        cursor_ = calculate_line_offset_add(lines_[y - 1], x);
    }

    // TODO: Update camera up.
    // TODO: Update camera horizontal.
}

void edit::BufferComponent::handle_cursor_down()
{
    auto y = get_cursor_y();
    if (y < lines_.size() - 1)
    {
        auto x = calculate_line_offset(lines_[y], cursor_);
        cursor_ = calculate_line_offset_add(lines_[y + 1], x);
    }

    // TODO: Update camera down.
    // TODO: Update camera horizontal.
}

void edit::BufferComponent::handle_cursor_left()
{
    if (cursor_ == 0)
        return;

    // TODO: Re-do this logic to move left from the cursor.
    std::size_t best = 0;

    // Move our cursor to the closest on-the-left visible character.
    for (std::size_t i = 0; i < cursor_; i++)
        if (!buffer_[i].is_deleted)
            best = i;

    cursor_ = best;

    // TODO: Update camera left.
    // TODO: Update camera up.
}

void edit::BufferComponent::handle_cursor_right()
{
    if (cursor_ >= buffer_.size())
        return;

    // 1. If our cursor is on a visible character, it is hovering over that character. Move to the next visible
    //    character on-the-right.
    // 2. If our cursor is not on a visible character, it is already hovering over the next visible character. This
    //    means that we need to skip the first visible character we see on-the-right.
    bool skip = buffer_[cursor_].is_deleted;
    auto best = buffer_.size();
    for (auto next = cursor_ + 1; next < buffer_.size(); next++)
    {
        if (!buffer_[next].is_deleted)
        {
            if (skip)
            {
                skip = false;
                continue;
            }

            best = next;
            break;
        }
    }

    cursor_ = best;

    // TODO: Update camera right.
    // TODO: Update camera down.
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
std::size_t edit::BufferComponent::calculate_line_offset(const Line &line, std::size_t index) const
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
std::size_t edit::BufferComponent::calculate_line_offset_add(const Line &line, std::size_t count) const
{
    for (std::size_t i = line.begin; i < line.end; i++)
        if (!buffer_[i].is_deleted)
            if ((count--) == 0)
                return i;
    return line.end;
}
