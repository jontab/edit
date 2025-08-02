#include "edit-common/Buffer.hpp"
#include <gtest/gtest.h>

#define ROOT_CLOCK 0
#define ROOT_SITE -1

TEST(BufferTests, Find_WorksAsExpected)
{
    std::vector<edit::common::Char> data{
        {.clock = 0, .site = 0},
        {.clock = 1, .site = 0},
        {.clock = 2, .site = 0},
    };
    EXPECT_EQ(edit::common::find(data, {.clock = 0, .site = 0}), 0);
    EXPECT_EQ(edit::common::find(data, {.clock = 1, .site = 0}), 1);
    EXPECT_EQ(edit::common::find(data, {.clock = 2, .site = 0}), 2);

    EXPECT_EQ(edit::common::find(data, {.clock = 0, .site = 1}), -1);
    EXPECT_EQ(edit::common::find(data, {.clock = 1, .site = 1}), -1);
    EXPECT_EQ(edit::common::find(data, {.clock = 2, .site = 1}), -1);
    EXPECT_EQ(edit::common::find(data, {.clock = ROOT_CLOCK, .site = ROOT_SITE}), -1);
}

TEST(BufferTests, FindSortedPosition_WorksAsExpected)
{
    std::vector<edit::common::Char> data{
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 3},
        {.parent_clock = 0,          .parent_site = 3,         .clock = 1, .site = 3},
        {.parent_clock = 1,          .parent_site = 3,         .clock = 2, .site = 3},
    };
    std::vector<edit::common::Char> insert{
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 0}, // 0.
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 2}, // 3.
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 4}, // 6.
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 0}, // 1.
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 2}, // 3.
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 0}, // 2.
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 2}, // 3.
    };
    EXPECT_EQ(edit::common::find_sorted_position(data, insert[0]), 0); // (Depth = 0, SiblingIx = 0).
    EXPECT_EQ(edit::common::find_sorted_position(data, insert[1]), 3); // (Depth = 0, SiblingIx = 1).
    EXPECT_EQ(edit::common::find_sorted_position(data, insert[2]), 6); // (Depth = 0, SiblingIx = 2).

    EXPECT_EQ(edit::common::find_sorted_position(data, insert[3]), 1); // (Depth = 1, SiblingIx = 0).
    EXPECT_EQ(edit::common::find_sorted_position(data, insert[4]), 3); // (Depth = 1, SiblingIx = 1).

    EXPECT_EQ(edit::common::find_sorted_position(data, insert[5]), 2); // (Depth = 2, SiblingIx = 0).
    EXPECT_EQ(edit::common::find_sorted_position(data, insert[6]), 3); // (Depth = 2, SiblingIx = 1).
}

TEST(BufferTests, Insert_ReturnsFalse_WhenCharAlreadyExists)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
    });
    for (const auto &ch : buffer)
        EXPECT_FALSE(buffer.insert(ch));
}

TEST(BufferTests, Insert_ReturnsTrue)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 3},
        {.parent_clock = 0,          .parent_site = 3,         .clock = 1, .site = 3},
        {.parent_clock = 1,          .parent_site = 3,         .clock = 2, .site = 3},
    });
    std::vector<edit::common::Char> insert{
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 0},
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 2},
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 4},
    };
    for (auto &ch : insert)
        EXPECT_TRUE(buffer.insert(ch));

    EXPECT_EQ(*(buffer.begin() + 0), insert[0]); // 0.
    EXPECT_EQ(*(buffer.begin() + 4), insert[1]); // 3 + 1.
    EXPECT_EQ(*(buffer.begin() + 8), insert[2]); // 6 + 2.
}

TEST(BufferTests, Remove_ReturnsFalse_WhenCharDoesNotExist)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
    });
    EXPECT_FALSE(buffer.remove({.clock = 0, .site = 3}));
    EXPECT_FALSE(buffer.remove({.clock = 1, .site = 3}));
    EXPECT_FALSE(buffer.remove({.clock = 2, .site = 3}));
}

TEST(BufferTests, Remove_ReturnsFalse_WhenCharIsAlreadyDeleted)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1, .is_deleted = true},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1, .is_deleted = true},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1, .is_deleted = true},
    });
    for (const auto &ch : buffer)
        EXPECT_FALSE(buffer.remove(ch));
}

TEST(BufferTests, Remove_ReturnsTrue)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
    });
    for (const auto &ch : buffer)
        EXPECT_TRUE(buffer.remove(ch));
    for (const auto &ch : buffer)
        EXPECT_TRUE(ch.is_deleted);
}

TEST(BufferTests, Clock_WorksAsExpected)
{
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 0, .site = 1},
        {.parent_clock = 0,          .parent_site = 1,         .clock = 1, .site = 1},
        {.parent_clock = 1,          .parent_site = 1,         .clock = 2, .site = 1},
    });
    EXPECT_EQ(buffer.clock(), 2);
    buffer.insert({.parent_clock = 2, .parent_site = 1, .clock = 3, .site = 1});
    buffer.insert({.parent_clock = 3, .parent_site = 1, .clock = 4, .site = 1});
    buffer.insert({.parent_clock = 4, .parent_site = 1, .clock = 5, .site = 1});
    EXPECT_EQ(buffer.clock(), 5);
}

TEST(BufferTests, Buffer_SerializeToJson)
{
    // Arrange.
    edit::common::Buffer buffer({
        {.parent_clock = ROOT_CLOCK, .parent_site = ROOT_SITE, .clock = 9, .site = 1},
    });

    // Act.
    nlohmann::json j = buffer;

    // Assert.
    EXPECT_EQ(j.dump(), "{\"data\":[{\"c\":9,\"ch\":0,\"d\":false,\"pc\":0,\"ps\":-1,\"s\":1}]}");
}

TEST(BufferTests, Buffer_DeserializeFromJson)
{
    // Arrange & Act.
    nlohmann::json j = nlohmann::json::parse("{\"data\":[{\"c\":9,\"ch\":0,\"d\":false,\"pc\":0,\"ps\":-1,\"s\":1}]}");
    auto buffer = j.get<edit::common::Buffer>();

    // Assert.
    EXPECT_EQ(buffer.clock(), 9);

    auto ch = *(buffer.begin() + 0);
    EXPECT_EQ(ch.parent_clock, ROOT_CLOCK);
    EXPECT_EQ(ch.parent_site, ROOT_SITE);
    EXPECT_EQ(ch.clock, 9);
    EXPECT_EQ(ch.site, 1);
    EXPECT_EQ(ch.ch, '\0');
    EXPECT_EQ(ch.is_deleted, false);
}
