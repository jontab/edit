#pragma once

#include "INetworkComponent.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

namespace edit::net
{

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;

class BeastNetworkComponent : public INetworkComponent
{
    std::shared_ptr<asio::io_context> ioc_;
    ssl::context ctx_;
    websocket::stream<ssl::stream<tcp::socket>> ws_;

  public:
    BeastNetworkComponent(std::shared_ptr<asio::io_context> &&ioc, ssl::context &&ctx,
                          websocket::stream<ssl::stream<tcp::socket>> &&ws);
};

} // namespace edit::net
