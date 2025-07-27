#include "Editor.hpp"

edit::Editor::Editor(std::unique_ptr<IView> view)
    : view(std::move(view)), action_bus(), event_bus(), buffer_component(this->action_bus),
      status_component(this->event_bus), is_running(true)
{
    action_bus.on<Quit>([this](const auto &) { is_running = false; });
}

void edit::Editor::run()
{
    while (is_running)
    {
        view->render(this->buffer_component, this->status_component);
        if (auto action = view->poll(); action.has_value())
        {
            this->action_bus.publish(action.value());
        }
    }
}
