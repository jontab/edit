#pragma once

#include "core/Dispatcher.hpp"

namespace edit
{

class CommandInterpreter
{
    core::Dispatcher &dispatcher_;

  public:
    CommandInterpreter(core::Dispatcher &dispatcher);

  private:
    void handle(const core::CommandEnteredEvent &ev);
};

} // namespace edit
