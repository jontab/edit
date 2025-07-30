#pragma once

#include "INetworkComponent.hpp"
#include <memory>
#include <optional>

namespace edit::net
{

struct NetworkConnectResult
{
    bool is_success;
    std::string error;
    std::optional<std::unique_ptr<edit::net::INetworkComponent>> component;
};

} // namespace edit::net
