#pragma once

#include "IMessageHandler.hpp"
#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"

namespace edit::network
{

class JsonMessageHandler : public IMessageHandler
{
    core::Bus<core::Action> &action_bus_;

  public:
    JsonMessageHandler(decltype(action_bus_) action_bus);
    void process(const std::string_view &message) override;
    std::string serialize(const core::events::CharInserted &ev) override;
    std::string serialize(const core::events::CharDeleted &ev) override;
};

} // namespace edit::network
