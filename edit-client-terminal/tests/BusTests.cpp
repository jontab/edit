#include "core/Action.hpp"
#include "core/Bus.hpp"
#include "core/Event.hpp"
#include <gtest/gtest.h>

using namespace edit::core;

TEST(BusTests, PublishesActionsToMultipleSubscribers)
{
    // Arrange.
    edit::core::Bus<edit::core::Action> ab;
    bool was_cursor_up_called_1 = false;
    bool was_cursor_up_called_2 = false;
    bool was_cursor_down_called_1 = false;
    bool was_cursor_down_called_2 = false;
    ab.on<actions::CursorUp>([&](const auto &) { was_cursor_up_called_1 = true; });
    ab.on<actions::CursorUp>([&](const auto &) { was_cursor_up_called_2 = true; });
    ab.on<actions::CursorDown>([&](const auto &) { was_cursor_down_called_1 = true; });
    ab.on<actions::CursorDown>([&](const auto &) { was_cursor_down_called_2 = true; });

    // Act.
    ab.post(actions::CursorUp{});
    ab.post(actions::CursorDown{});
    ab.publish();

    // Assert.
    EXPECT_TRUE(was_cursor_up_called_1);
    EXPECT_TRUE(was_cursor_up_called_2);
    EXPECT_TRUE(was_cursor_down_called_1);
    EXPECT_TRUE(was_cursor_down_called_2);
}

TEST(BusTests, PublishesActionsAndEventsToMultipleSubscribers)
{
    // Arrange.
    edit::core::Bus<edit::core::Action> ab;
    edit::core::Bus<edit::core::Event> eb;
    bool was_action_called_1 = false;
    bool was_action_called_2 = false;
    bool was_event_called_1 = false;
    bool was_event_called_2 = false;
    ab.on<actions::CursorUp>([&](const auto &) { was_action_called_1 = true; });
    ab.on<actions::CursorUp>([&](const auto &) { was_action_called_2 = true; });
    eb.on<events::CommandEntered>([&](const auto &) { was_event_called_1 = true; });
    eb.on<events::CommandEntered>([&](const auto &) { was_event_called_2 = true; });

    // Act.
    ab.post(actions::CursorUp{});
    ab.publish();
    eb.post(events::CommandEntered{":quit"});
    eb.publish();

    // Assert.
    EXPECT_TRUE(was_action_called_1);
    EXPECT_TRUE(was_action_called_2);
    EXPECT_TRUE(was_event_called_1);
    EXPECT_TRUE(was_event_called_2);
}
