#pragma once

#include "CommandInterpreter.hpp"
#include "InputInterpreter.hpp"
#include "network/INetworkComponent.hpp"
#include "state/EditorStore.hpp"
#include "ui/IView.hpp"
#include <boost/asio.hpp>

namespace edit
{

class Editor
{
    std::shared_ptr<boost::asio::io_context> ioc_;
    std::unique_ptr<core::Bus<core::Action>> action_bus_;
    std::unique_ptr<core::Bus<core::Event>> event_bus_;
    std::thread ioc_thread_;
    bool is_running_;

    std::shared_ptr<network::INetworkComponent> network_component_;
    std::unique_ptr<ui::IView> view_;

    state::EditorStore store_;
    InputInterpreter input_interpreter_;
    CommandInterpreter command_interpreter_;

  public:
    Editor(decltype(ioc_) ioc,
        decltype(action_bus_) &&action_bus,
        decltype(event_bus_) &&event_bus,
        decltype(network_component_) network_component,
        decltype(view_) &&view);

    void run();
};

} // namespace edit
