#include "uWebSocketServer.hpp"

uWebSocketServer::uWebSocketServer(uWS::App *app)
    : app_(app)
{
}

void uWebSocketServer::send(void *ws, const std::string &message)
{
    if (ws)
        static_cast<uWSType *>(ws)->send(message, uWS::OpCode::TEXT);
}

void uWebSocketServer::broadcast(const std::string &topic, const std::string &message)
{
    app_->publish(topic, message, uWS::OpCode::TEXT);
}

void uWebSocketServer::subscribe(void *ws, const std::string &topic)
{
    if (ws)
        static_cast<uWSType *>(ws)->subscribe(topic);
}
