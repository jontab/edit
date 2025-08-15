#pragma once

#include "BufferView.hpp"
#include "IView.hpp"
#include "LayoutEngine.hpp"
#include "StatusView.hpp"
#include "state/EditorStore.hpp"
#include <memory>

namespace edit::ui
{

class TermboxView : public IView
{
    std::unique_ptr<IViewBackend> backend_;
    LayoutEngine layout_;
    BufferView buffer_;
    StatusView status_;

  public:
    TermboxView();

    void poll(core::Bus<core::Action> &action_bus, core::Bus<core::Event> &event_bus) override;
    void render(const state::EditorStore &store) override;
};

} // namespace edit::ui
