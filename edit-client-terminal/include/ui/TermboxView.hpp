#pragma once

#include "../Bus.hpp"
#include "ui/BufferView.hpp"
#include "ui/IView.hpp"
#include "ui/LayoutEngine.hpp"
#include "ui/StatusView.hpp"
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
