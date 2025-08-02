#include "components/StatusComponent.hpp"
#include <gtest/gtest.h>

TEST(StatusComponentTests, GetDisplayCursor_WorksAsExpected)
{
    // Arrange.
    edit::ActionBus acbus;
    edit::EventBus evbus;
    edit::StatusComponent status{acbus, evbus};
    EXPECT_EQ(status.display_cursor_index(), 0);
    EXPECT_EQ(status.display_cursor_y(), 0);
    EXPECT_EQ(status.display_cursor_x(), 0);

    // Act.
    evbus.publish(edit::CursorMoved{.new_index = 3, .new_y = 2, .new_x = 1});

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
    edit::StatusComponent status{acbus, evbus};
    EXPECT_EQ(status.status(), "");

    // Act.
    acbus.publish(edit::Status{.text = "Example!"});

    // Assert.
    EXPECT_EQ(status.status(), "Example!");
}
