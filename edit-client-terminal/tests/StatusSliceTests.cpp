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
    status.reduce(actions::Insert{'c'});
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":c");

    // Insert a 'b' before that.
    status.reduce(actions::Insert{'b'});
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Move all the way to the left.
    status.reduce(actions::CursorLeft{});
    status.reduce(actions::CursorLeft{});
    status.reduce(actions::CursorLeft{});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Insert an 'a' at the beginning.
    status.reduce(actions::Insert{'a'});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, "a:bc");

    // Delete that 'a'.
    status.reduce(actions::Delete{});
    EXPECT_EQ(status.state().command_cursor, 0);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Delete from the very end (does nothing).
    status.reduce(actions::CursorRight{});
    status.reduce(actions::CursorRight{});
    status.reduce(actions::CursorRight{});
    status.reduce(actions::Delete{});
    EXPECT_EQ(status.state().command_cursor, 3);
    EXPECT_EQ(status.state().command_content, ":bc");

    // Delete the last character.
    status.reduce(actions::CursorLeft{});
    status.reduce(actions::Delete{});
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
    status.reduce(actions::Insert{'a'});
    status.reduce(actions::Insert{'a'});
    status.reduce(actions::Insert{'a'});
    status.reduce(actions::CursorRight{});
    status.reduce(actions::CursorRight{});
    status.reduce(actions::CursorRight{});
    EXPECT_EQ(status.state().command_cursor, 4);
    EXPECT_EQ(status.state().command_content, ":aaa");

    status.reset_command();
    EXPECT_EQ(status.state().command_cursor, 1);
    EXPECT_EQ(status.state().command_content, ":");
}
