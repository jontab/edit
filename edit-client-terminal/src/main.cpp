#include "Editor.hpp"
#include "ui/TermboxView.hpp"
#include <exception>

int main(int argc, char **argv)
{
    try
    {
        edit::Editor editor{std::make_unique<edit::ui::TermboxView>()};
        editor.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "The program has encountered a fatal exception." << std::endl;
        std::cerr << "Ex: " << error.what() << std::endl;
    }

    return 0;
}
