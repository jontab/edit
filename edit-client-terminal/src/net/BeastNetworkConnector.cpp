#include "net/BeastNetworkConnector.hpp"
#include "net/BeastNetworkComponent.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

using namespace edit::net;

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;

BeastNetworkConnector::BeastNetworkConnector()
{
}

NetworkConnectResult BeastNetworkConnector::connect(const std::string &host, const std::string &port,
                                                    const std::string &path, const std::string &apikey)
{
    auto ioc = std::make_shared<asio::io_context>();
    ssl::context ctx(ssl::context::tlsv12_client);

    // Use OS-dependent configuration for default certificate authority paths.
    boost::system::error_code ec;
    ctx.set_default_verify_paths(ec);
    if (ec.failed())
        return {false, ec.message(), std::nullopt};
    ctx.set_verify_mode(ssl::verify_peer); // Reject self-signed certificates.
    try
    {
        websocket::stream<ssl::stream<tcp::socket>> ws{*ioc, ctx};
        tcp::resolver resolver{*ioc};
        auto results = resolver.resolve(host, port);
        asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str()))
        {
            // Supposedly, required by many servers.
            return {false, "Failed to set SNI hostname", std::nullopt};
        }

        ws.next_layer().handshake(ssl::stream_base::client);

        // We need `Authorization: Bearer` on the request.
        ws.set_option(websocket::stream_base::decorator([apikey](websocket::request_type &req) {
            req.set(http::field::user_agent, "edit-client-terminal edit-client-terminal/1.0");
            if (!apikey.empty())
                req.set(http::field::authorization, "Bearer " + apikey);
        }));
        ws.handshake(host, path);

        return {true, "", std::make_unique<BeastNetworkComponent>(std::move(ioc), std::move(ctx), std::move(ws))};
    }
    catch (const boost::system::system_error &error)
    {
        return {false, error.what(), std::nullopt};
    }
}
