#include "ui/TermboxView.hpp"
#include "ui/TermboxBackend.hpp"

using namespace edit::ui;

TermboxView::TermboxView()
    : backend_(std::make_unique<TermboxBackend>()), layout_({backend_->height(), backend_->width()}),
      buffer_(std::make_unique<BufferView>()), status_(std::make_unique<StatusView>())
{
}

void TermboxView::poll(ActionBus &action_bus, EventBus &event_bus)
{
    backend_->poll(action_bus, event_bus, [this](auto height, auto width) { layout_.resize(height, width); });
}

void TermboxView::render(const BufferComponent &buffer, const StatusComponent &status)
{
    backend_->clear();
    buffer_->render(*backend_, buffer, layout_.buffer_rect());
    status_->render(*backend_, status, layout_.status_rect());
    backend_->flush();
}
