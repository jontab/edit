#pragma once

#include "core/Dispatcher.hpp"
#include "state/EditorStore.hpp"

namespace edit
{

class InputInterpreter
{
    core::Dispatcher &dispatcher_;
    const state::EditorStore &store_;

  public:
    InputInterpreter(core::Dispatcher &dispatcher, const state::EditorStore &store);

  private:
    void handle(const core::KeyPressedEvent &ev);
    void handle_normal_mode(const core::KeyPressedEvent &ev);
};

} // namespace edit
