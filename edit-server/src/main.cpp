#include "BufferSessionManager.hpp"
#include "FileSystemDatabase.hpp"
#include "uWebSocketServer.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    uWS::App app;

    auto db = std::make_unique<FileSystemDatabase>("./documents/");
    auto wss = std::make_unique<uWebSocketServer>(&app);
    auto manager = std::make_shared<BufferSessionManager>(std::move(db), std::move(wss));

    app.ws<PerSocketData>("/api/v1/:docid/ws",
           {.compression = uWS::SHARED_COMPRESSOR,
               .maxPayloadLength = 16 * 1024 * 1024,
               .idleTimeout = 16,
               .maxBackpressure = 1 * 1024 * 1024,
               .closeOnBackpressureLimit = false,
               .resetIdleTimeoutOnSend = false,
               .sendPingsAutomatically = true,
               .upgrade =
                   [](auto *res, auto *req, auto *context) {
                       PerSocketData data{std::string{req->getParameter(0)}};
                       res->template upgrade<PerSocketData>(std::move(data),
                           req->getHeader("sec-websocket-key"),
                           req->getHeader("sec-websocket-protocol"),
                           req->getHeader("sec-websocket-extensions"),
                           context);
                   },
               .open = [manager](auto *ws) { manager->on_client_connected(ws, ws->getUserData()->docid); },
               .message = [manager](auto *ws,
                              std::string_view message,
                              uWS::OpCode) { manager->on_client_message(ws, std::string{message}); },
               .close = [manager](auto *ws, int, std::string_view) { manager->on_client_disconnected(ws); }})
        .listen(3000,
            [](auto *listener) {
                if (listener)
                {
                    std::cout << "Listening on port " << 3000 << std::endl;
                }
            })
        .run();
    return 0;
}
