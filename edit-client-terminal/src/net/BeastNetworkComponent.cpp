#include "net/BeastNetworkComponent.hpp"

using namespace edit::net;

BeastNetworkComponent::BeastNetworkComponent(std::shared_ptr<asio::io_context> &&ioc, ssl::context &&ctx,
                                             websocket::stream<ssl::stream<tcp::socket>> &&ws)
    : ioc_(std::move(ioc)), ctx_(std::move(ctx)), ws_(std::move(ws))
{
}
