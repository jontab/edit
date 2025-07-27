#pragma once

#include "BufferComponent.hpp"
#include "StatusComponent.hpp"
#include <optional>

namespace edit
{

class IView
{
  public:
    virtual ~IView() = default;
    virtual std::optional<Action> poll() = 0;
    virtual void render(const BufferComponent &buffer, const StatusComponent &status) = 0;
};

class DefaultView : public IView
{
    struct Window
    {
        int y;
        int x;
        int w;
        int h;
    };

    Window buffer_window;
    Window status_window;

  public:
    DefaultView();
    ~DefaultView() override;
    std::optional<Action> poll() override;
    void render(const BufferComponent &buffer, const StatusComponent &status) override;

  private:
    void handle_resize();
    void render_buffer_component(const BufferComponent &buffer);
    void render_status_component(const StatusComponent &buffer);
    void calculate_buffer_window(int height, int width);
    void calculate_status_window(int height, int width);
};

} // namespace edit
