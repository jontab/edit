#include "state/StatusSlice.hpp"
#include <gtest/gtest.h>

using namespace edit::core;
using namespace edit::state;

TEST(StatusSliceTests, Command_EditMixed)
{
    // Arrange.
    StatusSlice status{};
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":");

    // Insert a 'c' at the end.
    status.reduce(InsertAction{'c'});
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":c");

    // Insert a 'b' before that.
    status.reduce(InsertAction{'b'});
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Move all the way to the left.
    status.reduce(CursorLeftAction{});
    status.reduce(CursorLeftAction{});
    status.reduce(CursorLeftAction{});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Insert an 'a' at the beginning.
    status.reduce(InsertAction{'a'});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, "a:bc");

    // Delete that 'a'.
    status.reduce(DeleteAction{});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Delete from the very end (does nothing).
    status.reduce(CursorRightAction{});
    status.reduce(CursorRightAction{});
    status.reduce(CursorRightAction{});
    status.reduce(DeleteAction{});
    EXPECT_EQ(status.state().command_cursor, 3);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Delete the last character.
    status.reduce(CursorLeftAction{});
    status.reduce(DeleteAction{});
    EXPECT_EQ(status.state().command_cursor, 2);
    EXPECT_EQ(status.state().command_content, ":b");
}

TEST(StatusComponentTests, ResetCommand_WorksAsExpected)
{
    // Arrange.
    StatusSlice status{};
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":");

    // Act & Assert.
    status.reduce(InsertAction{'a'});
    status.reduce(InsertAction{'a'});
    status.reduce(InsertAction{'a'});
    status.reduce(CursorRightAction{});
    status.reduce(CursorRightAction{});
    status.reduce(CursorRightAction{});
    EXPECT_EQ(status.state().command_cursor, 4);
    EXPECT_EQ(status.state().command_content, ":aaa");

    status.reset_command();
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":");
}
