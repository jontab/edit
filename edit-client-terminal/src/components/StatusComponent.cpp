#include "components/StatusComponent.hpp"

edit::StatusComponent::StatusComponent(Dispatcher &dispatcher, ModeComponent &mode_component)
    : dispatcher_(dispatcher)
    , mode_component_(mode_component)
    , display_cursor_index_()
    , display_cursor_y_()
    , display_cursor_x_()
    , status_()
    , command_cursor_(0)
    , command_content_("")
{
    dispatcher.on_event<CursorMovedEvent>([this](const auto &ev) { handle_cursor_moved(ev); });
    dispatcher.on_event<ModeChangedEvent>([this](const auto &ev) { handle_mode_changed(ev); });
    dispatcher.on_action<StatusAction>([this](const auto &ev) { status_ = ev.text; });
    dispatcher.on_action<CursorLeftAction>([this](const auto &) { handle_cursor_left(); });
    dispatcher.on_action<CursorRightAction>([this](const auto &) { handle_cursor_right(); });
    dispatcher.on_action<InsertAction>([this](const auto &ev) { handle_insert(ev); });
    dispatcher.on_action<DeleteAction>([this](const auto &) { handle_delete(); });
    dispatcher.on_action<BackspaceAction>([this](const auto &) { handle_backspace(); });
    dispatcher.on_action<EscapeAction>([this](const auto &ev) { handle_escape(); });
}

std::size_t edit::StatusComponent::display_cursor_index() const
{
    return display_cursor_index_;
}

std::size_t edit::StatusComponent::display_cursor_y() const
{
    return display_cursor_y_;
}

std::size_t edit::StatusComponent::display_cursor_x() const
{
    return display_cursor_x_;
}

const std::string &edit::StatusComponent::status() const
{
    return status_;
}

std::size_t edit::StatusComponent::command_cursor() const
{
    return command_cursor_;
}

const std::string &edit::StatusComponent::command_content() const
{
    return command_content_;
}

edit::Mode edit::StatusComponent::mode() const
{
    return mode_component_.mode();
}

void edit::StatusComponent::handle_cursor_moved(const CursorMovedEvent &ev)
{
    display_cursor_index_ = ev.new_index;
    display_cursor_y_ = ev.new_y;
    display_cursor_x_ = ev.new_x;
}

void edit::StatusComponent::handle_mode_changed(const ModeChangedEvent &ev)
{
    if (ev.to == Mode::CommandMode)
    {
        command_cursor_ = 1;
        command_content_ = ":";
    }
}

void edit::StatusComponent::handle_escape()
{
    if (mode_component_.mode() == Mode::CommandMode)
        dispatcher_.dispatch(edit::ChangeModeAction{Mode::NormalMode});
}

void edit::StatusComponent::handle_cursor_left()
{
    if (mode_component_.mode() != Mode::CommandMode)
        return;

    if (command_cursor_ > 0)
        command_cursor_--;
}

void edit::StatusComponent::handle_cursor_right()
{
    if (mode_component_.mode() != Mode::CommandMode)
        return;

    if (command_cursor_ < command_content_.length())
        command_cursor_ = command_content_.length();
}

void edit::StatusComponent::handle_insert(const InsertAction &ev)
{
    if (mode_component_.mode() != Mode::CommandMode)
        return;

    if (auto ch = static_cast<char>(ev.ch); ch == '\n')
    {
        dispatcher_.dispatch(edit::CommandAction{command_content_});
        dispatcher_.dispatch(edit::ChangeModeAction{Mode::NormalMode});
    }
    else if (isprint(ch))
    {
        auto l_end = command_cursor_; // Includes `command_cursor_`.
        auto r_beg = command_cursor_;

        auto l = command_content_.substr(0, l_end);
        auto r = (r_beg < command_content_.length()) ? command_content_.substr(r_beg) : "";
        command_content_ = l + static_cast<char>(ch) + r;
    }
}

void edit::StatusComponent::handle_delete()
{
    if (mode_component_.mode() != Mode::CommandMode)
        return;

    auto l_end = command_cursor_;     // Excludes `command_cursor`.
    auto r_beg = command_cursor_ + 1; // Excludes `command_cursor`.

    auto l = command_content_.substr(0, l_end);
    auto r = (r_beg < command_content_.length()) ? command_content_.substr(r_beg) : "";
    command_content_ = l + r;
}

void edit::StatusComponent::handle_backspace()
{
    if (mode_component_.mode() != Mode::CommandMode)
        return;

    if (command_cursor_ > 0)
    {
        handle_cursor_left();
        handle_delete();
    }
}
