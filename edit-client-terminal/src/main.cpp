#include "Editor.hpp"
#include "ViewContext.hpp"
#include <exception>

int main(int argc, char **argv)
{
    edit::Editor editor;
    try
    {
        edit::ViewContext ctx;
        editor.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "The program has encountered a fatal exception." << std::endl;
        std::cerr << "Ex: " << error.what() << std::endl;
    }

    return 0;
}
