#include "Bootstrap.hpp"
#include "Args.hpp"
#include "Editor.hpp"
#include "core/Bus.hpp"
#include "network/BeastNetworkComponent.hpp"
#include "ui/TermboxView.hpp"
#include <boost/asio.hpp>
#include <iostream>

int edit::Bootstrap::run(int argc, char **argv)
{
    Args args(argc, argv);

    auto ioc = std::make_shared<boost::asio::io_context>();
    auto action_bus = std::make_shared<ActionBus>();
    auto event_bus = std::make_shared<EventBus>();

    // `INetworkComponent`.
    auto network_component = std::make_shared<edit::network::BeastNetworkComponent>(ioc);
    if (!args.docid.empty())
    {
        std::cout << "Connecting to " << args.host << ":" << args.port << ". Please wait." << std::endl;
        auto future = network_component->connect(args.host, args.port, "/api/v1/" + args.docid + "/ws", args.apikey);
        ioc->run();
        ioc->restart();
        if (auto result = future.get(); !result.is_success)
        {
            std::cerr << "Failed to connect. Ex: " << result.error << std::endl;
            return EXIT_FAILURE;
        }
    }

    network_component->bind(*action_bus, *event_bus);

    // `IView`.
    auto view = std::make_unique<edit::ui::TermboxView>();
    try
    {
        edit::Editor editor{ioc, action_bus, event_bus, network_component, std::move(view)};
        editor.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "The program has encountered a fatal exception. Ex: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
