#pragma once

#include "IMessageHandler.hpp"
#include "INetworkComponent.hpp"
#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"
#include "core/EventTypes.hpp"
#include <edit-common/Delta.hpp>

#include <deque>
#include <future>
#include <memory>
#include <optional>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/websocket/stream.hpp>

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
    core::Bus<core::Action> &action_bus_;

    std::string host_;
    std::string port_;
    std::string path_;
    std::string apikey_;

    std::shared_ptr<asio::io_context> ioc_;
    ssl::context ctx_;
    websocket::stream<ssl::stream<tcp::socket>> ws_;
    tcp::resolver resolver_;
    boost::beast::flat_buffer buffer_;

    boost::asio::steady_timer reconnect_timer_;
    int reconnect_attempts_;

    std::shared_ptr<std::promise<NetworkConnectResult>> connect_promise_;

    bool is_writing_;
    std::deque<std::shared_ptr<std::string>> write_queue_;

    std::unique_ptr<IMessageHandler> message_handler_;

  public:
    BeastNetworkComponent(decltype(action_bus_) action_bus,
        core::Bus<core::Event> &event_bus,
        std::shared_ptr<asio::io_context> ioc,
        std::unique_ptr<IMessageHandler> &&handler);

    std::future<NetworkConnectResult> connect(const std::string &host,
        const std::string &port,
        const std::string &path,
        const std::string &apikey) override;

  private:
    void do_resolve();
    void do_connect(tcp::resolver::results_type results);
    void do_ssl_handshake();
    void do_websocket_handshake();
    void do_read();
    void do_write();

    void on_connect_failure(const std::string &message);
    void on_connect_success();

    void on_local_event(const core::events::CharDeleted &ev);
    void on_local_event(const core::events::CharInserted &ev);
    void on_local_event(const std::string &serialized);

    void reconnect();
    void reset();
};

} // namespace edit::network
