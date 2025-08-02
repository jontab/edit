#pragma once

#include "core/Bus.hpp"
#include "core/Point.hpp"
#include <cstdint>
#include <functional>
#include <string>

namespace edit::ui
{

/**
 * @brief Responsible for implementing low-level user-interface methods.
 */
class IViewBackend
{
  public:
    virtual ~IViewBackend() = default;

    virtual void clear() = 0;
    virtual void flush() = 0;
    virtual void put_char(unsigned int y, unsigned int x, std::uint32_t ch) = 0;
    virtual void put_text(unsigned int y, unsigned int x, std::string text) = 0;
    virtual void set_cursor(unsigned int y, unsigned int x) = 0;
    virtual unsigned int height() = 0;
    virtual unsigned int width() = 0;
    virtual void poll(ActionBus &action_bus,
        EventBus &event_bus,
        const std::function<void(unsigned int, unsigned int)> &on_resize) = 0;
};

} // namespace edit::ui
