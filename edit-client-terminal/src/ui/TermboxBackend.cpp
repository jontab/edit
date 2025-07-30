#define TB_IMPL

#include "ui/TermboxBackend.hpp"
#include "termbox2.h"

using namespace edit::ui;

TermboxBackend::TermboxBackend()
{
    if (tb_init() == TB_ERR)
        throw std::runtime_error("tb_init failed");
}

TermboxBackend::~TermboxBackend()
{
    tb_shutdown();
}

void TermboxBackend::clear()
{
    if (tb_clear() == TB_ERR)
        throw std::runtime_error("tb_clear failed");
}

void TermboxBackend::flush()
{
    if (tb_present() == TB_ERR)
        throw std::runtime_error("tb_present failed");
}

void TermboxBackend::put_char(unsigned int y, unsigned int x, std::uint32_t ch)
{
    if (tb_set_cell(x, y, ch, TB_WHITE, TB_DEFAULT) == TB_ERR)
        throw std::runtime_error("tb_set_cell failed");
}

void TermboxBackend::put_text(unsigned int y, unsigned int x, std::string text)
{
    for (std::size_t i = 0; i < text.size(); i++)
        if (tb_set_cell(x + i, y, static_cast<std::uint32_t>(text[i]), TB_WHITE, TB_DEFAULT) == TB_ERR)
            throw std::runtime_error("tb_set_cell failed");
}

void TermboxBackend::set_cursor(unsigned int y, unsigned int x)
{
    if (tb_set_cursor(x, y) == TB_ERR)
        throw std::runtime_error("tb_set_cursor");
}

unsigned int TermboxBackend::height()
{
    if (auto height = tb_height(); height >= 0)
    {
        return static_cast<unsigned int>(height);
    }
    else
    {
        throw std::runtime_error("tb_height failed");
    }
}

unsigned int TermboxBackend::width()
{
    if (auto width = tb_width(); width >= 0)
    {
        return static_cast<unsigned int>(width);
    }
    else
    {
        throw std::runtime_error("tb_width failed");
    }
}

void TermboxBackend::poll(ActionBus &action_bus, const std::function<void(unsigned int, unsigned int)> &on_resize)
{
    struct tb_event ev;
    switch (tb_peek_event(&ev, 50))
    {
    case TB_OK:
        break;
    default:
        return;
    }

    switch (ev.type)
    {
    case TB_EVENT_KEY:
        on_key(ev, action_bus);
        break;
    case TB_EVENT_RESIZE:
        on_resize(height(), width());
        break;
    default:
        break;
    }
}

void TermboxBackend::on_key(const struct tb_event &ev, ActionBus &action_bus)
{
    if (!ev.mod && !ev.key && isprint(ev.ch))
    {
        if (ev.ch == 'q')
            action_bus.publish(edit::Quit{});
        action_bus.publish(edit::Insert{ev.ch});
        action_bus.publish(edit::CursorRight{});
        return;
    }

    switch (ev.key)
    {
    case TB_KEY_ENTER:
        action_bus.publish(edit::Insert{'\n'});
        action_bus.publish(edit::CursorRight{});
        return;
    case TB_KEY_TAB:
        action_bus.publish(edit::Insert{'\t'});
        action_bus.publish(edit::CursorRight{});
        return;
    case TB_KEY_ARROW_UP:
        action_bus.publish(edit::CursorUp{});
        return;
    case TB_KEY_ARROW_DOWN:
        action_bus.publish(edit::CursorDown{});
        return;
    case TB_KEY_ARROW_LEFT:
        action_bus.publish(edit::CursorLeft{});
        return;
    case TB_KEY_ARROW_RIGHT:
        action_bus.publish(edit::CursorRight{});
        return;
    case TB_KEY_DELETE:
        action_bus.publish(edit::Delete{});
        return;
    case TB_KEY_BACKSPACE:
    case TB_KEY_BACKSPACE2:
        action_bus.publish(edit::Backspace{});
        return;
    default:
        break;
    }
}
