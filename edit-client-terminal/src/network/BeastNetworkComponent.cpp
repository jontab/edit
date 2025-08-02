#include "network/BeastNetworkComponent.hpp"
#include <iostream>

using namespace edit::network;

BeastNetworkComponent::BeastNetworkComponent(std::shared_ptr<asio::io_context> ioc)
    : host_()
    , port_()
    , path_()
    , apikey_()
    , ioc_(ioc)
    , ctx_(ssl::context::tlsv12_client)
    , ws_(*ioc, ctx_)
    , resolver_(*ioc)
    , reconnect_timer_(*ioc)
    , reconnect_attempts_(0)
    , buffer_()
    , connect_promise_()
    , action_bus_()
    , event_bus_()
{
    // Use OS-dependent configuration for default certificate authority paths.
    boost::system::error_code ec;
    ctx_.set_default_verify_paths(ec);
    if (ec.failed())
    {
        throw std::runtime_error(ec.message());
    }

    // Reject self-signed certificates if an environment variable is not set.
    if (std::getenv("ALLOW_SELF_SIGNED_CERTIFICATES") == nullptr)
    {
        ctx_.set_verify_mode(ssl::verify_peer);
    }
}

/**
 * @brief Connects to the WebSocket server with the given parameters.
 *
 * This is meant to be called in `main`. Upon the initial success or failure, the future is set with the result, and the
 * `ioc` is stopped.
 *
 * @param host Host of WebSocket server.
 * @param port Port of WebSocket server.
 * @param path Path (e.g., /api/v1/blah/ws) of WebSocket server.
 * @param apikey Included in `Authentication: Bearer` header.
 * @return A future, resolved upon the first success or failure.
 */
std::future<NetworkConnectResult> BeastNetworkComponent::connect(const std::string &host,
    const std::string &port,
    const std::string &path,
    const std::string &apikey)
{
    connect_promise_ = std::make_shared<std::promise<NetworkConnectResult>>();
    host_ = host;
    port_ = port;
    path_ = path;
    apikey_ = apikey;
    do_resolve();
    return connect_promise_->get_future();
}

/**
 * @brief Binds `ActionBus` and `EventBus` to this component. Used for sending and receiving actions and events.
 */
void BeastNetworkComponent::bind(ActionBus &action_bus, EventBus &event_bus)
{
    action_bus_ = std::ref(action_bus);
    event_bus_ = std::ref(event_bus);
}

void BeastNetworkComponent::do_resolve()
{
    resolver_.async_resolve(
        host_, port_, [self = shared_from_this()](boost::system::error_code ec, tcp::resolver::results_type results) {
            if (ec.failed())
                return self->on_connect_failure("resolve: " + ec.message());
            self->do_connect(results);
        });
}

void BeastNetworkComponent::do_connect(tcp::resolver::results_type results)
{
    boost::asio::async_connect(
        ws_.next_layer().next_layer(), results, [self = shared_from_this()](boost::system::error_code ec, auto) {
            if (ec.failed())
                return self->on_connect_failure("connect: " + ec.message());
            self->do_ssl_handshake();
        });
}

void BeastNetworkComponent::do_ssl_handshake()
{
    // Set SNI hostname, required for many servers.
    if (!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host_.c_str()))
        return on_connect_failure("Failed to set SNI hostname");
    ws_.next_layer().async_handshake(
        ssl::stream_base::client, [self = shared_from_this()](boost::system::error_code ec) {
            if (ec.failed())
                return self->on_connect_failure("ssl_handshake: " + ec.message());
            self->do_websocket_handshake();
        });
}

void BeastNetworkComponent::do_websocket_handshake()
{
    // We need `Authorization: Bearer` on the request.
    ws_.set_option(websocket::stream_base::decorator([self = shared_from_this()](websocket::request_type &req) {
        req.set(http::field::user_agent, "edit-client-terminal edit-client-terminal/1.0");
        if (!self->apikey_.empty())
            req.set(http::field::authorization, "Bearer " + self->apikey_);
    }));
    ws_.async_handshake(host_, path_, [self = shared_from_this()](boost::system::error_code ec) {
        if (ec)
            return self->on_connect_failure("websocket_handshake: " + ec.message());
        if (self->connect_promise_)
        {
            self->connect_promise_->set_value({true, ""});
            self->connect_promise_.reset();
            self->ioc_->stop();
        }
        else
        {
            self->on_connect_success();
        }
    });
}

void BeastNetworkComponent::do_read()
{
    ws_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
        if (ec.failed())
            return self->on_connect_failure("read: " + ec.message());
        // std::string message = boost::beast::buffers_to_string(self->buffer_.data());
        self->buffer_.consume(bytes);
        self->do_read();
    });
}

void BeastNetworkComponent::reconnect()
{
    reset();
    reconnect_attempts_++;
    auto delay = std::chrono::seconds(5);
    reconnect_timer_.expires_at(std::chrono::steady_clock::now() + delay);
    reconnect_timer_.async_wait([self = shared_from_this()](boost::system::error_code ec) {
        if (ec == boost::asio::error::operation_aborted)
            return;
        self->do_resolve();
    });
}

void BeastNetworkComponent::reset()
{
    if (ws_.is_open())
        ws_.next_layer().shutdown();
    new (&ws_) websocket::stream<ssl::stream<tcp::socket>>(*ioc_, ctx_);
}

void BeastNetworkComponent::on_connect_success()
{
    reconnect_attempts_ = 0;
    do_read();
}

void BeastNetworkComponent::on_connect_failure(const std::string &message)
{
    if (connect_promise_)
    {
        connect_promise_->set_value({false, message});
        connect_promise_.reset();
        ioc_->stop();
    }
    else
    {
        reconnect();
    }
}
