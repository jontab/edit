#include "state/StatusSlice.hpp"

using namespace edit::core;
using namespace edit::state;

StatusSlice::StatusSlice()
    : s_()
{
    reset_command();
}

const StatusState &StatusSlice::state() const
{
    return s_;
}

/******************************************************************************/
/* Reduce                                                                     */
/******************************************************************************/

void StatusSlice::reduce(const actions::CursorLeft &a)
{
    if (s_.command_cursor > 0)
        s_.command_cursor--;
}

void StatusSlice::reduce(const actions::CursorRight &a)
{
    if (s_.command_cursor < s_.command_content.length())
        s_.command_cursor = s_.command_content.length();
}

std::optional<events::CommandEntered> StatusSlice::reduce(const actions::Insert &a)
{
    if (auto ch = static_cast<char>(a.ch); ch == '\n')
    {
        return events::CommandEntered{s_.command_content};
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

void StatusSlice::reduce(const actions::Delete &a)
{
    auto l_end = s_.command_cursor;     // Excludes `command_cursor`.
    auto r_beg = s_.command_cursor + 1; // Excludes `command_cursor`.
    auto l = s_.command_content.substr(0, l_end);
    auto r = (r_beg < s_.command_content.length()) ? s_.command_content.substr(r_beg) : "";
    s_.command_content = l + r;
}

void StatusSlice::reduce(const actions::Backspace &a)
{
    if (s_.command_cursor > 0)
    {
        reduce(actions::CursorLeft{});
        reduce(actions::Delete{});
    }
}

void StatusSlice::reduce(const actions::ChangeStatus &a)
{
    s_.status = a.message;
}

/******************************************************************************/
/* Other                                                                      */
/******************************************************************************/

void StatusSlice::reset_command()
{
    s_.command_content = ":";
    s_.command_cursor = 1;
}
