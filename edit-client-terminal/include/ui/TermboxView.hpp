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
    std::unique_ptr<BufferView> buffer_;
    std::unique_ptr<StatusView> status_;

  public:
    TermboxView();

    void poll(ActionBus &action_bus) override;
    void render(const BufferComponent &buffer, const StatusComponent &status) override;
};

} // namespace edit::ui
