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
    std::unique_ptr<Dispatcher> dispatcher_;

    // Components.
    std::shared_ptr<edit::network::INetworkComponent> network_component_;
    EditorStore store_;

    // View.
    std::unique_ptr<ui::IView> view_;

    // State.
    bool is_running_;

  public:
    Editor(std::shared_ptr<boost::asio::io_context> ioc,
        std::unique_ptr<Dispatcher> &&dispatcher,
        std::shared_ptr<edit::network::INetworkComponent> network_component,
        std::unique_ptr<ui::IView> &&view);

    void run();

  private:
    void handle(const CommandEnteredEvent &e);
    void handle(const KeyPressedEvent &e);
};

} // namespace edit
