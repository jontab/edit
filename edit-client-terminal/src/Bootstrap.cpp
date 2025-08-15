#include "Bootstrap.hpp"
#include "Args.hpp"
#include "Editor.hpp"
#include "network/BeastNetworkComponent.hpp"
#include "network/JsonMessageHandler.hpp"
#include "ui/TermboxView.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace std::chrono_literals;
using namespace edit::core;
using namespace edit::network;
using namespace edit::ui;

int edit::Bootstrap::run(int argc, char **argv)
{
    Args args(argc, argv);

    auto action_bus = std::make_unique<core::Bus<core::Action>>();
    auto event_bus = std::make_unique<core::Bus<core::Event>>();
    auto ioc = std::make_shared<boost::asio::io_context>();

    // `INetworkComponent`.
    auto network_component = std::make_shared<BeastNetworkComponent>(
        *action_bus, *event_bus, ioc, std::make_unique<JsonMessageHandler>(*action_bus));
    if (!args.docid.empty())
    {
        std::cout << "Connecting to " << args.host << ":" << args.port << ". Please wait." << std::endl;

        auto future = network_component->connect(args.host, args.port, "/api/v1/" + args.docid + "/ws", args.apikey);
        while (future.wait_for(0s) != std::future_status::ready)
            ioc->run_one_for(10ms);
        if (auto result = future.get(); !result.is_success)
        {
            std::cerr << "Failed to connect. Ex: " << result.error << std::endl;
            return EXIT_FAILURE;
        }
    }

    // `IView`.
    auto view = std::make_unique<TermboxView>();
    try
    {
        edit::Editor editor{ioc, std::move(action_bus), std::move(event_bus), network_component, std::move(view)};
        editor.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "The program has encountered a fatal exception. Ex: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
