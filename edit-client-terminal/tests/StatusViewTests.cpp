#include "MockBackend.hpp"
#include "ui/LayoutEngine.hpp"
#include "ui/StatusView.hpp"
#include <gtest/gtest.h>

using namespace edit::core;
using namespace edit::state;
using namespace edit::ui;

void check_text(const MockBackend &backend, unsigned int y, unsigned int x, std::string text)
{
    for (std::size_t i = 0; i < text.size(); i++)
        EXPECT_EQ(backend.get_char_at(y, x + i), text[i]);
}

TEST(StatusViewTests, Render_WorksAsExpected)
{
    // Arrange.
    StatusSlice status{};
    auto backend = std::make_unique<MockBackend>(10, 30);
    LayoutEngine layout{backend->height(), backend->width()};
    StatusView view{};

    backend->clear();
    view.render(*backend,
        {
            .slice = status,
            .current_mode = Mode::NormalMode,
            .buffer_cursor_index = 0,
            .buffer_cursor_y = 0,
            .buffer_cursor_x = 0,
        },
        layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "Normal | (I:0, Y:0, X:0)");

    // Assert.
    backend->clear();
    view.render(*backend,
        {
            .slice = status,
            .current_mode = Mode::NormalMode,
            .buffer_cursor_index = 100,
            .buffer_cursor_y = 200,
            .buffer_cursor_x = 300,
        },
        layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "Normal | (I:100, Y:200, X:300)");
}

TEST(StatusViewTests, Render_WorksAsExpected_WithStatus)
{
    // Arrange.
    StatusSlice status{};
    auto backend = std::make_unique<MockBackend>(10, 30);
    LayoutEngine layout{backend->height(), backend->width()};
    StatusView view{};

    backend->clear();
    view.render(*backend,
        {
            .slice = status,
            .current_mode = Mode::NormalMode,
            .buffer_cursor_index = 0,
            .buffer_cursor_y = 0,
            .buffer_cursor_x = 0,
        },
        layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "Normal | (I:0, Y:0, X:0)");

    // Act.
    status.reduce(ChangeStatusAction{"ab"});

    // Assert.
    backend->clear();
    view.render(*backend,
        {
            .slice = status,
            .current_mode = Mode::NormalMode,
            .buffer_cursor_index = 0,
            .buffer_cursor_y = 0,
            .buffer_cursor_x = 0,
        },
        layout.status_rect());
    check_text(*backend, backend->height() - 1, 0, "Normal | (I:0, Y:0, X:0) | ab");
}
