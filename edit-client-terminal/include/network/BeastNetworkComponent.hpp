#pragma once

#include "INetworkComponent.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <future>
#include <optional>

namespace edit::network
{

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;

class BeastNetworkComponent
    : public INetworkComponent
    , public std::enable_shared_from_this<BeastNetworkComponent>
{
    // Connection parameters, passed to `connect`.
    std::string host_;
    std::string port_;
    std::string path_;
    std::string apikey_;

    // Asynchronous state.
    std::shared_ptr<asio::io_context> ioc_;
    ssl::context ctx_;
    websocket::stream<ssl::stream<tcp::socket>> ws_;
    tcp::resolver resolver_;
    boost::asio::steady_timer reconnect_timer_;
    int reconnect_attempts_;
    boost::beast::flat_buffer buffer_;
    std::shared_ptr<std::promise<NetworkConnectResult>> connect_promise_;

    // `Dispatcher`, populated in `bind`.
    std::optional<std::reference_wrapper<edit::core::Dispatcher>> dispatcher_;

  public:
    BeastNetworkComponent(std::shared_ptr<asio::io_context> ioc);

    std::future<NetworkConnectResult> connect(const std::string &host,
        const std::string &port,
        const std::string &path,
        const std::string &apikey) override;
    void bind(edit::core::Dispatcher &dispatcher) override;

  private:
    void do_resolve();
    void do_connect(tcp::resolver::results_type results);
    void do_ssl_handshake();
    void do_websocket_handshake();
    void do_read();

    void on_connect_failure(const std::string &message);
    void on_connect_success();

    void reconnect();
    void reset();
};

} // namespace edit::network
