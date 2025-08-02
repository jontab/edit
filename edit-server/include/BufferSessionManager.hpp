#pragma once

#include "BufferSession.hpp"
#include "IDatabase.hpp"
#include "IWebSocketServer.hpp"
#include <map>
#include <memory>

class BufferSessionManager
{
  private:
    std::unique_ptr<IDatabase> db_;
    std::unique_ptr<IWebSocketServer> wss_;

    std::map<std::string, BufferSession> sessions_; // Maps from `docid`.
    std::map<void *, std::string> users_;           // Maps to `docid`.

  public:
    BufferSessionManager(std::unique_ptr<IDatabase> db, std::unique_ptr<IWebSocketServer> wss);

    void on_client_connected(void *ws, const std::string &docid);
    void on_client_message(void *ws, const std::string &message);
    void on_client_disconnected(void *ws);
};
