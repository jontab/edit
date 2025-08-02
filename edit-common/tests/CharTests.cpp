#include "edit-common/Char.hpp"
#include <gtest/gtest.h>

#define JSON_A "{\"c\":2,\"ch\":97,\"d\":false,\"pc\":0,\"ps\":1,\"s\":3}"
#define JSON_B "{\"c\":6,\"ch\":98,\"d\":true,\"pc\":4,\"ps\":5,\"s\":7}"

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

TEST(CharTests, Char_SerializeToJson)
{
    // Arrange.
    edit::common::Char a{0, 1, 2, 3, 'a', false};
    edit::common::Char b{4, 5, 6, 7, 'b', true};

    // Act.
    nlohmann::json ja = a;
    nlohmann::json jb = b;

    // Assert.
    EXPECT_EQ(ja.dump(), JSON_A);
    EXPECT_EQ(jb.dump(), JSON_B);
}

TEST(CharTests, Char_DeserializeFromJson)
{
    // Arrange & Act.
    edit::common::Char a = nlohmann::json::parse(JSON_A).get<edit::common::Char>();
    edit::common::Char b = nlohmann::json::parse(JSON_B).get<edit::common::Char>();

    // Assert.
    EXPECT_EQ(a.parent_clock, 0);
    EXPECT_EQ(a.parent_site, 1);
    EXPECT_EQ(a.clock, 2);
    EXPECT_EQ(a.site, 3);
    EXPECT_EQ(a.ch, 'a');
    EXPECT_EQ(a.is_deleted, false);

    EXPECT_EQ(b.parent_clock, 4);
    EXPECT_EQ(b.parent_site, 5);
    EXPECT_EQ(b.clock, 6);
    EXPECT_EQ(b.site, 7);
    EXPECT_EQ(b.ch, 'b');
    EXPECT_EQ(b.is_deleted, true);
}
