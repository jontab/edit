#pragma once

#include "core/Dispatcher.hpp"
#include "core/Mode.hpp"

namespace edit
{

class ModeComponent
{
    Dispatcher &dispatcher_;
    Mode mode_;

  public:
    ModeComponent(Dispatcher &dispatcher);

    Mode mode() const;

  private:
    void handle_key_pressed(const KeyPressedEvent &ev);
    void handle_change_mode(Mode mode);
};

} // namespace edit
