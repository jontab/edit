#pragma once

#include "BufferView.hpp"
#include "IView.hpp"
#include "LayoutEngine.hpp"
#include "StatusView.hpp"
#include "core/Bus.hpp"
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

    void poll(Dispatcher &dispatcher) override;
    void render(const EditorStore &store) override;
};

} // namespace edit::ui
