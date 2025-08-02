#include "components/StatusComponent.hpp"
#include <gtest/gtest.h>

TEST(StatusComponentTests, GetDisplayCursor_WorksAsExpected)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    edit::ModeComponent mode{dispatcher};
    edit::StatusComponent status{dispatcher, mode};
    EXPECT_EQ(status.display_cursor_index(), 0);
    EXPECT_EQ(status.display_cursor_y(), 0);
    EXPECT_EQ(status.display_cursor_x(), 0);

    // Act.
    dispatcher.emit(edit::CursorMovedEvent{.new_index = 3, .new_y = 2, .new_x = 1});

    // Assert.
    EXPECT_EQ(status.display_cursor_index(), 3);
    EXPECT_EQ(status.display_cursor_y(), 2);
    EXPECT_EQ(status.display_cursor_x(), 1);
}

TEST(StatusComponentTests, GetStatus_WorksAsExpected)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    edit::ModeComponent mode{dispatcher};
    edit::StatusComponent status{dispatcher, mode};
    EXPECT_EQ(status.status(), "");

    // Act.
    dispatcher.dispatch(edit::StatusAction{"Example!"});

    // Assert.
    EXPECT_EQ(status.status(), "Example!");
}

TEST(StatusComponentTests, Command_EditMixed)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    edit::ModeComponent mode{dispatcher};
    edit::StatusComponent status{dispatcher, mode};
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::InsertMode});
    EXPECT_EQ(status.mode(), edit::Mode::InsertMode);
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "");

    // Act & Assert.
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::CommandMode});
    EXPECT_EQ(status.mode(), edit::Mode::CommandMode);
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":");

    // Insert a 'c' at the end.
    dispatcher.dispatch(edit::InsertAction{'c'});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":c");

    // Insert a 'b' before that.
    dispatcher.dispatch(edit::InsertAction{'b'});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":bc");

    // Move all the way to the left.
    dispatcher.dispatch(edit::CursorLeftAction{});
    dispatcher.dispatch(edit::CursorLeftAction{});
    dispatcher.dispatch(edit::CursorLeftAction{});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), ":bc");

    // Insert an 'a' at the beginning.
    dispatcher.dispatch(edit::InsertAction{'a'});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "a:bc");

    // Delete that 'a'.
    dispatcher.dispatch(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), ":bc");

    // Delete from the very end (does nothing).
    dispatcher.dispatch(edit::CursorRightAction{});
    dispatcher.dispatch(edit::CursorRightAction{});
    dispatcher.dispatch(edit::CursorRightAction{});
    dispatcher.dispatch(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 3);
    EXPECT_EQ(status.command_content(), ":bc");

    // Delete the last character.
    dispatcher.dispatch(edit::CursorLeftAction{});
    dispatcher.dispatch(edit::DeleteAction{});
    EXPECT_EQ(status.command_cursor(), 2);
    EXPECT_EQ(status.command_content(), ":b");
}

TEST(StatusComponentTests, Command_ResetsWhenModeChanged)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    edit::ModeComponent mode{dispatcher};
    edit::StatusComponent status{dispatcher, mode};
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::InsertMode});
    EXPECT_EQ(status.mode(), edit::Mode::InsertMode);
    EXPECT_EQ(status.command_cursor(), 0);
    EXPECT_EQ(status.command_content(), "");

    // Act & Assert.
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::CommandMode});
    dispatcher.dispatch(edit::InsertAction{'a'});
    dispatcher.dispatch(edit::InsertAction{'a'});
    dispatcher.dispatch(edit::InsertAction{'a'});
    dispatcher.dispatch(edit::CursorRightAction{});
    dispatcher.dispatch(edit::CursorRightAction{});
    dispatcher.dispatch(edit::CursorRightAction{});
    EXPECT_EQ(status.command_cursor(), 4);
    EXPECT_EQ(status.command_content(), ":aaa");

    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::InsertMode});
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::CommandMode});
    EXPECT_EQ(status.command_cursor(), 1);
    EXPECT_EQ(status.command_content(), ":");
}

TEST(StatusComponentTests, Command_ToNormalOnEscape)
{
    // Arrange.
    edit::Dispatcher dispatcher;
    edit::ModeComponent mode{dispatcher};
    edit::StatusComponent status{dispatcher, mode};
    dispatcher.dispatch(edit::ChangeModeAction{edit::Mode::CommandMode});

    // Act & Assert.
    dispatcher.dispatch(edit::EscapeAction{});
    EXPECT_EQ(status.mode(), edit::Mode::NormalMode);
}
