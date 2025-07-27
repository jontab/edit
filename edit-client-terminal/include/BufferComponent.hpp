#pragma once

#include "Bus.hpp"

namespace edit
{

class BufferComponent
{
  public:
    BufferComponent(ActionBus &action_bus);

    int get_camera_y();
    int get_camera_x();

  private:
    void handle_cursor_up();
    void handle_cursor_down();
    void handle_cursor_left();
    void handle_cursor_right();
};

} // namespace edit
