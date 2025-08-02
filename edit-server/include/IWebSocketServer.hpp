#pragma once

#include <string>

class IWebSocketServer
{
  public:
    virtual ~IWebSocketServer() = default;

    virtual void send(void *ws, const std::string &message) = 0;
    virtual void broadcast(const std::string &docid, const std::string &message) = 0;
    virtual void subscribe(void *ws, const std::string &topic) = 0;
};
