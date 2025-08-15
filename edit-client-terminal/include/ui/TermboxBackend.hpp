#pragma once

#include "IViewBackend.hpp"
#include <cstdint>
#include <functional>
#include <string_view>

struct tb_event;

namespace edit::ui
{

class TermboxBackend : public IViewBackend
{
  public:
    TermboxBackend();
    ~TermboxBackend() override;

    void clear() override;
    void flush() override;
    void put_char(unsigned int y, unsigned int x, std::uint32_t ch) override;
    void put_text(unsigned int y, unsigned int x, const std::string_view &text) override;
    void set_cursor(unsigned int y, unsigned int x) override;
    unsigned int height() override;
    unsigned int width() override;

    void poll(core::Bus<core::Action> &action_bus,
        core::Bus<core::Event> &event_bus,
        const std::function<void(unsigned int, unsigned int)> &on_resize) override;

  private:
    void on_key(const struct tb_event &ev, core::Bus<core::Action> &action_bus, core::Bus<core::Event> &event_bus);
};

} // namespace edit::ui
