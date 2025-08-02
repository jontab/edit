#include "App.h"
#include "IWebSocketServer.hpp"
#include "PerSocketData.hpp"

/**
 * @brief This wraps `uWS::WebSocket` so that we can test without `uWS`.
 */
class uWebSocketServer : public IWebSocketServer
{
    uWS::App *app_;

    using uWSType = uWS::WebSocket<false, true, PerSocketData>;

  public:
    uWebSocketServer(uWS::App *app);

    void send(void *ws, const std::string &message) override;
    void broadcast(const std::string &topic, const std::string &message) override;
    void subscribe(void *ws, const std::string &topic) override;
};
