#include "components/StatusComponent.hpp"
#include <gtest/gtest.h>

TEST(StatusComponentTests, GetDisplayCursor_WorksAsExpected)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::ModeComponent mode{acbus, evbus};
    edit::StatusComponent status{acbus, evbus, mode};
    EXPECT_EQ(status.display_cursor_index(), 0);
    EXPECT_EQ(status.display_cursor_y(), 0);
    EXPECT_EQ(status.display_cursor_x(), 0);

    // Act.
    evbus.publish(edit::CursorMovedEvent{.new_index = 3, .new_y = 2, .new_x = 1});

    // Assert.
    EXPECT_EQ(status.display_cursor_index(), 3);
    EXPECT_EQ(status.display_cursor_y(), 2);
    EXPECT_EQ(status.display_cursor_x(), 1);
}

TEST(StatusComponentTests, GetStatus_WorksAsExpected)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::ModeComponent mode{acbus, evbus};
    edit::StatusComponent status{acbus, evbus, mode};
    EXPECT_EQ(status.status(), "");

    // Act.
    acbus.publish(edit::StatusAction{"Example!"});

    // Assert.
    EXPECT_EQ(status.status(), "Example!");
}

TEST(StatusComponentTests, Command_EditMixed)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::ModeComponent mode{acbus, evbus};
    edit::StatusComponent status{acbus, evbus, mode};
    acbus.publish(edit::ChangeModeAction{edit::Mode::InsertMode});
    EXPECT_EQ(status.mode(), edit::Mode::InsertMode);
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "");

    // Act & Assert.
    acbus.publish(edit::ChangeModeAction{edit::Mode::CommandMode});
    EXPECT_EQ(status.mode(), edit::Mode::CommandMode);
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":");

    // Insert a 'c' at the end.
    acbus.publish(edit::InsertAction{'c'});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":c");

    // Insert a 'b' before that.
    acbus.publish(edit::InsertAction{'b'});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":bc");

    // Move all the way to the left.
    acbus.publish(edit::CursorLeftAction{});
    acbus.publish(edit::CursorLeftAction{});
    acbus.publish(edit::CursorLeftAction{});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), ":bc");

    // Insert an 'a' at the beginning.
    acbus.publish(edit::InsertAction{'a'});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "a:bc");

    // Delete that 'a'.
    acbus.publish(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), ":bc");

    // Delete from the very end (does nothing).
    acbus.publish(edit::CursorRightAction{});
    acbus.publish(edit::CursorRightAction{});
    acbus.publish(edit::CursorRightAction{});
    acbus.publish(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 3);
    EXPECT_EQ(status.command_content(), ":bc");

    // Delete the last character.
    acbus.publish(edit::CursorLeftAction{});
    acbus.publish(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 2);
    EXPECT_EQ(status.command_content(), ":b");
}

TEST(StatusComponentTests, Command_ResetsWhenModeChanged)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::ModeComponent mode{acbus, evbus};
    edit::StatusComponent status{acbus, evbus, mode};
    acbus.publish(edit::ChangeModeAction{edit::Mode::InsertMode});
    EXPECT_EQ(status.mode(), edit::Mode::InsertMode);
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "");

    // Act & Assert.
    acbus.publish(edit::ChangeModeAction{edit::Mode::CommandMode});
    acbus.publish(edit::InsertAction{'a'});
    acbus.publish(edit::InsertAction{'a'});
    acbus.publish(edit::InsertAction{'a'});
    acbus.publish(edit::CursorRightAction{});
    acbus.publish(edit::CursorRightAction{});
    acbus.publish(edit::CursorRightAction{});
    EXPECT_EQ(status.command_cursor(), 4);
    EXPECT_EQ(status.command_content(), ":aaa");

    acbus.publish(edit::ChangeModeAction{edit::Mode::InsertMode});
    acbus.publish(edit::ChangeModeAction{edit::Mode::CommandMode});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":");
}

TEST(StatusComponentTests, Command_ToNormalOnEscape)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::ModeComponent mode{acbus, evbus};
    edit::StatusComponent status{acbus, evbus, mode};
    acbus.publish(edit::ChangeModeAction{edit::Mode::CommandMode});

    // Act & Assert.
    acbus.publish(edit::EscapeAction{});
    EXPECT_EQ(status.mode(), edit::Mode::NormalMode);
}
