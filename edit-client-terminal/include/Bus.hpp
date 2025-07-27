#pragma once

#include "Action.hpp"
#include "Event.hpp"
#include <functional>
#include <map>
#include <memory>
#include <typeindex>

namespace edit
{

template <typename Message> class Bus
{
    // Lets us store handlers for different types in the same container.
    class IHandler
    {
      public:
        virtual ~IHandler() = default;
        virtual void dispatch(const Message &message) = 0;
    };

    template <typename T> class Handler : public IHandler
    {
        std::function<void(const T &)> callback;

      public:
        explicit Handler(std::function<void(const T &)> type) : callback(std::move(type))
        {
        }

        void dispatch(const Message &message) override
        {
            callback(std::get<T>(message));
        }
    };

    std::map<std::type_index, std::vector<std::unique_ptr<IHandler>>> subscribers;

  public:
    template <typename T> void on(std::function<void(const T &)> handler)
    {
        subscribers[typeid(T)].push_back(std::make_unique<Handler<T>>(std::move(handler)));
    }

    void publish(const Message &message)
    {
        // `std::visit` is idiomatic to do something different for each type of a `std::variant`.
        std::visit(
            [this](const auto &specific) {
                // Q: What's the point of `std::decay_t`?
                // A: `decltype(specific)` might return `const CursorUp &`. If we try to look that up in our subscribers
                //    list, we're not going to find anything, because we registered the handlers to `CursorUp`. It takes
                //    our `const CursorUp &` and turns it into `CursorUp`.
                using T = std::decay_t<decltype(specific)>;
                auto it = subscribers.find(typeid(T));
                if (it != subscribers.end())
                    for (const auto &handler : it->second)
                        handler->dispatch(specific);
            },
            message);
    }
};

using ActionBus = Bus<Action>;
using EventBus = Bus<Event>;

} // namespace edit
