#pragma once

#include "network/INetworkComponent.hpp"
#include "state/EditorStore.hpp"
#include "ui/IView.hpp"
#include <boost/asio.hpp>

namespace edit
{

class Editor
{
    // Core.
    std::shared_ptr<boost::asio::io_context> ioc_;
    std::unique_ptr<core::Dispatcher> dispatcher_;

    // Components.
    std::shared_ptr<network::INetworkComponent> network_component_;
    edit::state::EditorStore store_;

    // View.
    std::unique_ptr<ui::IView> view_;

    // State.
    bool is_running_;

  public:
    Editor(std::shared_ptr<boost::asio::io_context> ioc,
        std::unique_ptr<core::Dispatcher> &&dispatcher,
        std::shared_ptr<network::INetworkComponent> network_component,
        std::unique_ptr<ui::IView> &&view);

    void run();

  private:
    void handle(const core::CommandEnteredEvent &e);
    void handle(const core::KeyPressedEvent &e);
};

} // namespace edit
