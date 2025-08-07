#include "ui/TermboxView.hpp"
#include "ui/TermboxBackend.hpp"

edit::ui::TermboxView::TermboxView()
    : backend_(std::make_unique<TermboxBackend>())
    , layout_({backend_->height(), backend_->width()})
    , buffer_()
    , status_()
{
}

void edit::ui::TermboxView::poll(Dispatcher &dispatcher)
{
    backend_->poll(dispatcher, [this](auto height, auto width) { layout_.resize(height, width); });
}

void edit::ui::TermboxView::render(const EditorStore &store)
{
    backend_->clear();
    buffer_.render(*backend_, store.buffer(), layout_.buffer_rect(), store.mode().mode());
    status_.render(*backend_, store, layout_.status_rect(), store.mode().mode());
    backend_->flush();
}
