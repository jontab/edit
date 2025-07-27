#include "edit-common/Char.hpp"
#include <gtest/gtest.h>

TEST(CharTests, CharEquality)
{
    edit::common::Char a{.parent_clock = 0, .parent_site = 0, .clock = 1, .site = 1, .ch = 'a', .is_deleted = false};
    edit::common::Char b{.parent_clock = 1, .parent_site = 1, .clock = 1, .site = 1, .ch = 'b', .is_deleted = true};
    edit::common::Char c{.parent_clock = 2, .parent_site = 2, .clock = 2, .site = 2, .ch = 'c', .is_deleted = false};
    EXPECT_EQ(a, a);
    EXPECT_EQ(b, b);
    EXPECT_EQ(c, c);

    EXPECT_EQ(a, b);
    EXPECT_NE(b, c);
    EXPECT_NE(a, c);
}
