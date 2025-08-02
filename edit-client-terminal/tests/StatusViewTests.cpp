#include "MockBackend.hpp"
#include "components/StatusComponent.hpp"
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
    // // Arrange.
    // edit::ActionBus acbus;
    // edit::EventBus evbus;
    // edit::ModeComponent mode{acbus, evbus};
    // edit::StatusComponent status{acbus, evbus, mode};
    // auto backend = std::make_unique<MockBackend>(10, 30);
    // edit::ui::LayoutEngine layout{backend->height(), backend->width()};
    // edit::ui::StatusView view;

    // backend->clear();
    // view.render(*backend, status, layout.status_rect());
    // check_text(*backend, backend->height() - 1, 0, "(I:0, Y:0, X:0)");

    // // Act.
    // evbus.publish(edit::CursorMovedEvent{100, 200, 300});

    // // Assert.
    // backend->clear();
    // view.render(*backend, status, layout.status_rect());
    // check_text(*backend, backend->height() - 1, 0, "(I:100, Y:200, X:300)");
}

TEST(StatusViewTests, Render_WorksAsExpected_WithStatus)
{
    // // Arrange.
    // edit::ActionBus acbus;
    // edit::EventBus evbus;
    // edit::ModeComponent mode{acbus, evbus};
    // edit::StatusComponent status{acbus, evbus, mode};
    // auto backend = std::make_unique<MockBackend>(10, 30);
    // edit::ui::LayoutEngine layout{backend->height(), backend->width()};
    // edit::ui::StatusView view;

    // backend->clear();
    // view.render(*backend, status, layout.status_rect());
    // check_text(*backend, backend->height() - 1, 0, "(I:0, Y:0, X:0)");

    // // Act.
    // acbus.publish(edit::StatusAction{"Example!"});

    // // Assert.
    // backend->clear();
    // view.render(*backend, status, layout.status_rect());
    // check_text(*backend, backend->height() - 1, 0, "(I:0, Y:0, X:0) | Example!");
}
