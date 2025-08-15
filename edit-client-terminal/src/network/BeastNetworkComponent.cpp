#include "network/BeastNetworkComponent.hpp"
#include "core/ActionTypes.hpp"
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

using namespace edit::common;
using namespace edit::core;
using namespace edit::core::actions;
using namespace edit::core::events;
using namespace edit::network;

BeastNetworkComponent::BeastNetworkComponent(decltype(action_bus_) action_bus,
    core::Bus<core::Event> &event_bus,
    std::shared_ptr<asio::io_context> ioc,
    std::unique_ptr<IMessageHandler> &&message_handler)
    : action_bus_(action_bus)
    , host_()
    , port_()
    , path_()
    , apikey_()
    , ioc_(ioc)
    , ctx_(ssl::context::tlsv12_client)
    , ws_(*ioc, ctx_)
    , resolver_(*ioc)
    , buffer_()
    , reconnect_timer_(*ioc)
    , reconnect_attempts_(0)
    , connect_promise_()
    , is_writing_(false)
    , write_queue_()
    , message_handler_(std::move(message_handler))
{
    event_bus.on<CharInserted>([this](const auto &ev) { on_local_event(ev); });
    event_bus.on<CharDeleted>([this](const auto &ev) { on_local_event(ev); });

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
            self->do_read();
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

        auto data = self->buffer_.data();
        auto text = boost::beast::buffers_to_string(data);
        self->message_handler_->process(text);

        self->buffer_.consume(bytes);
        self->do_read();
    });
}

void BeastNetworkComponent::do_write()
{
    ws_.async_write(
        asio::buffer(*write_queue_.front()), [self = shared_from_this()](boost::system::error_code ec, std::size_t) {
            if (ec)
            {
                self->is_writing_ = false;
                return self->on_connect_failure("write: " + ec.message());
            }

            // Discard the message when we have successfully sent it.
            self->write_queue_.pop_front();

            // Continue to write until there are no messages left.
            if (!self->write_queue_.empty())
            {
                self->do_write();
            }
            else
            {
                self->is_writing_ = false;
            }
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
    }
    else
    {
        reconnect();
    }
}

void BeastNetworkComponent::on_local_event(const core::events::CharDeleted &ev)
{
    on_local_event(message_handler_->serialize(ev));
}

void BeastNetworkComponent::on_local_event(const core::events::CharInserted &ev)
{
    on_local_event(message_handler_->serialize(ev));
}

void BeastNetworkComponent::on_local_event(const std::string &serialized)
{
    write_queue_.push_back(std::make_shared<std::string>(serialized));

    // Start writing if we aren't already.
    if (!is_writing_)
    {
        is_writing_ = true;
        do_write();
    }
}
