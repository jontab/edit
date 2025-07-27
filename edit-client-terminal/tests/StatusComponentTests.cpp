#include "StatusComponent.hpp"
#include <gtest/gtest.h>

TEST(StatusComponentTests, GetDisplayCursor_WorksAsExpected)
{
    edit::EventBus bus;
    edit::StatusComponent status{bus};
    EXPECT_EQ(status.get_display_cursor_index(), 0);
    EXPECT_EQ(status.get_display_cursor_y(), 0);
    EXPECT_EQ(status.get_display_cursor_x(), 0);

    bus.publish(edit::Event{
        edit::CursorMoved{.new_index = 3, .new_y = 2, .new_x = 1}
    });

    EXPECT_EQ(status.get_display_cursor_index(), 3);
    EXPECT_EQ(status.get_display_cursor_y(), 2);
    EXPECT_EQ(status.get_display_cursor_x(), 1);
}
