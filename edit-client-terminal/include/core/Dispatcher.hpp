#pragma once

#include "Bus.hpp"

namespace edit
{

class Dispatcher
{
    ActionBus action_bus_;
    EventBus event_bus_;

  public:
    inline Dispatcher()
        : action_bus_()
        , event_bus_()
    {
    }

    template <typename T> void dispatch(const T &action)
    {
        action_bus_.publish(action);
    }

    template <typename T> void emit(const T &event)
    {
        event_bus_.publish(event);
    }

    template <typename T, typename Func> void on_action(Func &&handler)
    {
        action_bus_.on<T>(std::forward<Func>(handler));
    }

    template <typename T, typename Func> void on_event(Func &&handler)
    {
        event_bus_.on<T>(std::forward<Func>(handler));
    }
};

} // namespace edit
