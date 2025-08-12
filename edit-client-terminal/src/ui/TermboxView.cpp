#include "ui/TermboxView.hpp"
#include "ui/TermboxBackend.hpp"

using namespace edit::core;
using namespace edit::state;
using namespace edit::ui;

TermboxView::TermboxView()
    : backend_(std::make_unique<TermboxBackend>())
    , layout_({backend_->height(), backend_->width()})
    , buffer_()
    , status_()
{
}

void TermboxView::poll(Dispatcher &dispatcher)
{
    backend_->poll(dispatcher, [this](auto height, auto width) { layout_.resize(height, width); });
}

void TermboxView::render(const EditorStore &store)
{
    backend_->clear();

    // Buffer.
    buffer_.render(*backend_,
        {
            .slice = store.buffer(),
            .cursor_y = store.buffer().get_cursor_position().y,
            .cursor_x = store.buffer().get_cursor_position().x,
            .current_mode = store.mode().mode(),
        },
        layout_.buffer_rect());

    // Status.
    status_.render(*backend_,
        {
            .slice = store.status(),
            .current_mode = store.mode().mode(),
            .buffer_cursor_index = store.buffer().cursor(),
            .buffer_cursor_y = store.buffer().get_cursor_position().y,
            .buffer_cursor_x = store.buffer().get_cursor_position().x,
        },
        layout_.status_rect());

    backend_->flush();
}
