#include "BufferSessionManager.hpp"
#include <iostream>

BufferSessionManager::BufferSessionManager(std::unique_ptr<IDatabase> db, std::unique_ptr<IWebSocketServer> wss)
    : db_(std::move(db)), wss_(std::move(wss)), sessions_(), users_()
{
}

/**
 * @brief Handle an incoming connection from a client.
 * @param ws Handle to the client.
 * @param docid The document they are connecting to.
 */
void BufferSessionManager::on_client_connected(void *ws, const std::string &docid)
{
    std::cout << "INFO: " << docid << ": User connected" << std::endl;
    if (auto it = sessions_.find(docid); it == sessions_.end())
    {
        std::cout << "INFO: " << docid << ": Session is new. Loading from database" << std::endl;
        if (auto buffer = db_->load(docid); buffer.has_value())
        {
            sessions_[docid] = BufferSession{.buffer = std::move(buffer.value())};
        }
        else
        {
            sessions_[docid] = BufferSession{};
        }
    }

    auto &session = sessions_.at(docid);
    session.users.insert(ws);
    users_[ws] = docid;

    wss_->subscribe(ws, docid);
    // TODO: wss_->send(ws, nlohmann::json{session.buffer}.dump());
}

/**
 * @brief Handle a message from a client.
 * @param ws Handle to the client.
 * @param message The message content.
 */
void BufferSessionManager::on_client_message(void *ws, const std::string &message)
{
    if (auto it = users_.find(ws); it != users_.end())
    {
        const std::string &docid = it->second;
        std::cout << "INFO: " << docid << ": " << message << std::endl;
        // TODO: auto &session = sessions_.at(docid);
        // TODO: Apply to copy.
        wss_->broadcast(docid, message);
    }
    else
    {
        std::cerr << "ERROR: Received message from unknown user" << std::endl;
    }
}

/**
 * @brief Handle a disconnection from a client.
 * @param ws Handle to the client.
 */
void BufferSessionManager::on_client_disconnected(void *ws)
{
    if (auto it = users_.find(ws); it != users_.end())
    {
        std::string docid = it->second; // Copy.
        if (auto session_it = sessions_.find(docid); session_it != sessions_.end())
        {
            auto &session = session_it->second;
            session.users.erase(ws);
            users_.erase(ws); // Because we're going to be manipulating this.
            std::cout << "INFO: " << docid << ": User disconnected" << std::endl;

            if (session.users.empty())
            {
                std::cout << "INFO: " << docid << ": Session is empty. Saving to database" << std::endl;
                db_->save(docid, session.buffer);
                sessions_.erase(session_it);
            }
        }
        else
        {
            users_.erase(it);
        }
    }
}
