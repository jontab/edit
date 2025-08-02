#pragma once

#include "ui/IViewBackend.hpp"
#include <unordered_map>

class MockBackend : public edit::ui::IViewBackend
{
    edit::ui::Point<unsigned int> dims_;
    edit::ui::Point<unsigned int> cursor_;
    std::unordered_map<edit::ui::Point<unsigned int>, std::uint32_t> pixels_;

  public:
    inline MockBackend(unsigned int height, unsigned int width)
        : dims_({height, width})
        , cursor_({0, 0})
    {
    }

    inline void clear() override
    {
        pixels_.clear();
    }

    inline void flush() override
    {
    }

    inline void put_char(unsigned int y, unsigned int x, std::uint32_t ch) override
    {
        if (y < dims_.y)
            pixels_[edit::ui::Point<unsigned int>{y, x}] = ch;
        // TODO: Maybe throw?
    }

    inline void put_text(unsigned int y, unsigned int x, std::string text) override
    {
        for (unsigned int i = 0; i < static_cast<unsigned int>(text.size()); i++)
            if ((y < dims_.y) && (x + i < dims_.x))
                pixels_[edit::ui::Point<unsigned int>{y, x + i}] = text[i];
        // TODO: Maybe throw?
    }

    inline void set_cursor(unsigned int y, unsigned int x) override
    {
        cursor_ = {y, x};
    }

    inline unsigned int height() override
    {
        return dims_.y;
    }

    inline unsigned int width() override
    {
        return dims_.x;
    }

    inline void poll(edit::ActionBus &action_bus,
        edit::EventBus &event_bus,
        const std::function<void(unsigned int, unsigned int)> &on_resize) override
    {
        action_bus.publish(edit::QuitAction{});
    }

    std::uint32_t get_char_at(unsigned int y, unsigned int x) const
    {
        edit::ui::Point<unsigned int> key{y, x};
        auto it = pixels_.find(key);
        if (it != pixels_.end())
        {
            return it->second;
        }
        else
        {
            return ' ';
        }
    }

    edit::ui::Point<unsigned int> get_cursor() const
    {
        return cursor_;
    }
};
