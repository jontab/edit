#include "MockBackend.hpp"
#include "StatusComponent.hpp"
#include "ui/LayoutEngine.hpp"
#include "ui/StatusView.hpp"
#include <gtest/gtest.h>

void check_text(const MockBackend &backend, unsigned int y, unsigned int x, std::string text)
{
    for (std::size_t i = 0; i < text.size(); i++)
        EXPECT_EQ(backend.get_char_at(y, x + i), text[i]);
}

TEST(StatusViewTests, Render_WorksAsExpected)
{
    edit::EventBus evbus;
    edit::StatusComponent status{evbus};
    auto backend = std::make_unique<MockBackend>(10, 30);
    edit::ui::LayoutEngine layout{backend->height(), backend->width()};
    edit::ui::StatusView view;

    backend->clear();
    view.render(*backend, status, layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "(I:0, Y:0, X:0)");

    evbus.publish(edit::CursorMoved{100, 200, 300});

    backend->clear();
    view.render(*backend, status, layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "(I:100, Y:200, X:300)");
}
