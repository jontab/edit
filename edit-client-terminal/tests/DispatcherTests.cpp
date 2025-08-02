#include "core/Dispatcher.hpp"
#include <gtest/gtest.h>

TEST(ActionBusTests, PublishesToMultipleSubscribers)
{
    // Arrange.
    edit::ActionBus bus;
    bool was_cursor_up_called_1 = false;
    bool was_cursor_up_called_2 = false;
    bool was_cursor_down_called_1 = false;
    bool was_cursor_down_called_2 = false;
    bus.on<edit::CursorUpAction>([&](const auto &) { was_cursor_up_called_1 = true; });
    bus.on<edit::CursorUpAction>([&](const auto &) { was_cursor_up_called_2 = true; });
    bus.on<edit::CursorDownAction>([&](const auto &) { was_cursor_down_called_1 = true; });
    bus.on<edit::CursorDownAction>([&](const auto &) { was_cursor_down_called_2 = true; });

    // Act.
    bus.publish(edit::Action{edit::CursorUpAction{}});
    bus.publish(edit::Action(edit::CursorDownAction{}));

    // Assert.
    EXPECT_TRUE(was_cursor_up_called_1);
    EXPECT_TRUE(was_cursor_up_called_2);
    EXPECT_TRUE(was_cursor_down_called_1);
    EXPECT_TRUE(was_cursor_down_called_2);
}

TEST(DispatcherTests, PublishesToMultipleSubscribers)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    bool was_action_called_1 = false;
    bool was_action_called_2 = false;
    bool was_event_called_1 = false;
    bool was_event_called_2 = false;
    dispatcher.on_action<edit::CursorUpAction>([&](const auto &) { was_action_called_1 = true; });
    dispatcher.on_action<edit::CursorUpAction>([&](const auto &) { was_action_called_2 = true; });
    dispatcher.on_event<edit::CursorMovedEvent>([&](const auto &) { was_event_called_1 = true; });
    dispatcher.on_event<edit::CursorMovedEvent>([&](const auto &) { was_event_called_2 = true; });

    // Act.
    dispatcher.dispatch(edit::CursorUpAction{});
    dispatcher.emit(edit::CursorMovedEvent{});

    // Assert.
    EXPECT_TRUE(was_action_called_1);
    EXPECT_TRUE(was_action_called_2);
    EXPECT_TRUE(was_event_called_1);
    EXPECT_TRUE(was_event_called_2);
}
