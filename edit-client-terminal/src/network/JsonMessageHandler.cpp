#include "network/JsonMessageHandler.hpp"
#include <edit-common/Char.hpp>
#include <edit-common/Delta.hpp>

namespace edit::network
{

JsonMessageHandler::JsonMessageHandler(decltype(action_bus_) action_bus)
    : action_bus_(action_bus)
{
}

void JsonMessageHandler::process(const std::string_view &message)
{
    try
    {
        auto j = nlohmann::json::parse(message);
        auto d = j.get<common::Delta>();
        if (d.is_delete)
        {
            action_bus_.post(core::actions::RemoteDelete{d.ch});
        }
        else
        {
            action_bus_.post(core::actions::RemoteInsert{d.ch});
        }
    }
    catch (const std::exception &e)
    {
        // TODO: More error-handling here?
        action_bus_.post(core::actions::ChangeStatus{e.what()});
    }
}

std::string JsonMessageHandler::serialize(const core::events::CharInserted &ev)
{
    common::Delta delta = {.is_delete = false, .ch = ev.ch};
    nlohmann::json j = delta;
    return j.dump();
}

std::string JsonMessageHandler::serialize(const core::events::CharDeleted &ev)
{
    common::Delta delta = {.is_delete = true, .ch = ev.ch};
    nlohmann::json j = delta;
    return j.dump();
}

} // namespace edit::network
