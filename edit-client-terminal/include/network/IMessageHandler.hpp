#pragma once

#include "core/EventTypes.hpp"
#include <string>
#include <string_view>

namespace edit::network
{

class IMessageHandler
{
  public:
    virtual ~IMessageHandler() = default;
    virtual void process(const std::string_view &message) = 0;
    virtual std::string serialize(const core::events::CharInserted &ev) = 0;
    virtual std::string serialize(const core::events::CharDeleted &ev) = 0;
};

} // namespace edit::network
