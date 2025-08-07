#include "state/StatusSlice.hpp"

edit::StatusSlice::StatusSlice()
    : s_()
{
    reset_command();
}

/******************************************************************************/
/* Reduce                                                                     */
/******************************************************************************/

void edit::StatusSlice::reduce(const CursorLeftAction &a)
{
    if (s_.command_cursor > 0)
        s_.command_cursor--;
}

void edit::StatusSlice::reduce(const CursorRightAction &a)
{
    if (s_.command_cursor < s_.command_content.length())
        s_.command_cursor = s_.command_content.length();
}

std::optional<edit::CommandEnteredEvent> edit::StatusSlice::reduce(const InsertAction &a)
{
    if (auto ch = static_cast<char>(a.ch); ch == '\n')
    {
        return CommandEnteredEvent{s_.command_content};
    }
    else if (isprint(ch))
    {
        auto l_end = s_.command_cursor; // Includes `s_.command_cursor`.
        auto r_beg = s_.command_cursor;
        auto l = s_.command_content.substr(0, l_end);
        auto r = (r_beg < s_.command_content.length()) ? s_.command_content.substr(r_beg) : "";
        s_.command_content = l + static_cast<char>(ch) + r;
    }

    return std::nullopt;
}

void edit::StatusSlice::reduce(const DeleteAction &a)
{
    auto l_end = s_.command_cursor;     // Excludes `command_cursor`.
    auto r_beg = s_.command_cursor + 1; // Excludes `command_cursor`.
    auto l = s_.command_content.substr(0, l_end);
    auto r = (r_beg < s_.command_content.length()) ? s_.command_content.substr(r_beg) : "";
    s_.command_content = l + r;
}

void edit::StatusSlice::reduce(const BackspaceAction &a)
{
    if (s_.command_cursor > 0)
    {
        reduce(CursorLeftAction{});
        reduce(DeleteAction{});
    }
}

/******************************************************************************/
/* Other                                                                      */
/******************************************************************************/

void edit::StatusSlice::reset_command()
{
    s_.command_content = ":";
    s_.command_cursor = 1;
}

const std::string &edit::StatusSlice::status() const
{
    return s_.status;
}

std::size_t edit::StatusSlice::command_cursor() const
{
    return s_.command_cursor;
}

const std::string &edit::StatusSlice::command_content() const
{
    return s_.command_content;
}
