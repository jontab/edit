#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <typeindex>

namespace edit::core
{

template <typename Message> class Bus
{
    class IHandler
    {
      public:
        virtual ~IHandler() = default;
        virtual void dispatch(const Message &message) = 0;
    };

    template <typename T> class Handler : public IHandler
    {
        std::function<void(const T &)> callback_;

      public:
        explicit Handler(std::function<void(const T &)> callback)
            : callback_(std::move(callback))
        {
        }

        void dispatch(const Message &message) override
        {
            callback_(std::get<T>(message));
        }
    };

    std::map<std::type_index, std::vector<std::unique_ptr<IHandler>>> subscribers_;
    std::queue<Message> queue_;
    std::mutex lock_;

  public:
    /**
     * @brief Registers a handler for a given message type.
     *
     * @tparam T Message type.
     *
     * @param handler Handler.
     */
    template <typename T> void on(std::function<void(const T &)> handler)
    {
        subscribers_[typeid(T)].push_back(std::make_unique<Handler<T>>(std::move(handler)));
    }

    /**
     * @brief Schedule a message to be published later, thread-safe.
     * @param message Message.
     */
    void post(const Message &message)
    {
        std::lock_guard<decltype(lock_)> lock(lock_);
        queue_.push(message);
    }

    /**
     * @brief Publishes all pending messages and executes their handlers on this thread, thread-safe.
     */
    void publish()
    {
        decltype(queue_) local;
        {
            std::lock_guard<decltype(lock_)> lock(lock_);
            std::swap(local, queue_);
        }

        while (!local.empty())
        {
            const auto &message = local.front();
            dispatch(message);
            local.pop();
        }
    }

  private:
    void dispatch(const Message &message)
    {
        std::visit(
            [this](const auto &specific) {
                using T = std::decay_t<decltype(specific)>;
                if (auto it = subscribers_.find(typeid(T)); it != subscribers_.end())
                    for (const auto &handler : it->second)
                        handler->dispatch(specific);
            },
            message);
    }
};

} // namespace edit::core
