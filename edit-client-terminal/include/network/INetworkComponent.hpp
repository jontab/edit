#pragma once

#include "core/Dispatcher.hpp"
#include <future>
#include <string>

namespace edit::network
{

struct NetworkConnectResult
{
    bool is_success;
    std::string error;
};

class INetworkComponent
{
  public:
    virtual ~INetworkComponent() = default;

    virtual std::future<NetworkConnectResult> connect(const std::string &host,
        const std::string &port,
        const std::string &path,
        const std::string &apikey) = 0;
    virtual void bind(edit::core::Dispatcher &dispatcher) = 0;
};

} // namespace edit::network
