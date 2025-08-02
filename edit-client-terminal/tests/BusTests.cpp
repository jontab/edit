#include "core/Bus.hpp"
#include <gtest/gtest.h>

TEST(ActionBusTests, Publish_WorksAsExpected)
{
    edit::ActionBus bus;
    bool was_cursor_up_called_1 = false;
    bool was_cursor_up_called_2 = false;
    bool was_cursor_down_called_1 = false;
    bool was_cursor_down_called_2 = false;

    bus.on<edit::CursorUpAction>([&was_cursor_up_called_1](const auto &) { was_cursor_up_called_1 = true; });
    bus.on<edit::CursorDownAction>([&was_cursor_down_called_1](const auto &) { was_cursor_down_called_1 = true; });
    bus.on<edit::CursorUpAction>([&was_cursor_up_called_2](const auto &) { was_cursor_up_called_2 = true; });
    bus.on<edit::CursorDownAction>([&was_cursor_down_called_2](const auto &) { was_cursor_down_called_2 = true; });
    bus.publish(edit::Action{edit::CursorUpAction{}});
    bus.publish(edit::Action(edit::CursorDownAction{}));

    EXPECT_TRUE(was_cursor_up_called_1);
    EXPECT_TRUE(was_cursor_up_called_2);
    EXPECT_TRUE(was_cursor_down_called_1);
    EXPECT_TRUE(was_cursor_down_called_2);
}
