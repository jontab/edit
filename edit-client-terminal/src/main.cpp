#include "Args.hpp"
#include "Editor.hpp"
#include "net/BeastNetworkConnector.hpp"
#include "ui/TermboxView.hpp"
#include <exception>

int main(int argc, char **argv)
{
    edit::Args args(argc, argv);
    std::cout << "Connecting to " << args.host << ":" << args.port << ", please wait." << std::endl;
    auto connector = std::make_unique<edit::net::BeastNetworkConnector>();
    auto result = connector->connect(args.host, args.port, "/", args.apikey);
    if (!result.is_success)
    {
        std::cerr << "Failed to connect. Ex:" << result.error << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        edit::Editor editor{std::make_unique<edit::ui::TermboxView>(), std::move(*result.component)};
        editor.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "The program has encountered a fatal exception. Ex: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
