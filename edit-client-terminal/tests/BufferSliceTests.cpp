#include "state/BufferSlice.hpp"
#include <gtest/gtest.h>

using namespace edit::common;
using namespace edit::core;
using namespace edit::state;

template <typename T>
void verify_cursor(BufferSlice &buffer, std::size_t index, const Point<std::size_t> &from, const Point<std::size_t> &to)
{
    buffer.set_cursor_index(index);
    EXPECT_EQ(buffer.get_cursor_position(), from);
    buffer.reduce(T{});
    EXPECT_EQ(buffer.get_cursor_position(), to);
}

TEST(BufferSliceTests, Site_IsRandom)
{
    srand(1337);
    BufferSlice a{};
    BufferSlice b{};
    EXPECT_NE(a.site(), b.site());
}

TEST(BufferSliceTests, CursorUp_WorksAsExpected)
{
    std::vector<Char> chars{
        {.ch = 'H',  .is_deleted = false}, // 0.
        {.ch = 'e',  .is_deleted = false},
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'o',  .is_deleted = false},
        {.ch = ',',  .is_deleted = false},
        {.ch = '\n', .is_deleted = false},
        {.ch = 'w',  .is_deleted = false}, // 7.
        {.ch = 'o',  .is_deleted = false},
        {.ch = 'r',  .is_deleted = true }, // Deleted.
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'd',  .is_deleted = false},
        {.ch = '!',  .is_deleted = false},
        {.ch = '!',  .is_deleted = false},
        {.ch = '!',  .is_deleted = false},
    };
    BufferSlice buf{std::move(chars)};
    verify_cursor<CursorUpAction>(buf, 7, {1, 0}, {0, 0});  // Moving up normally.
    verify_cursor<CursorUpAction>(buf, 0, {0, 0}, {0, 0});  // Moving up from first row is a no-op.
    verify_cursor<CursorUpAction>(buf, 9, {1, 2}, {0, 2});  // Moving up from a deleted character.
    verify_cursor<CursorUpAction>(buf, 15, {1, 7}, {0, 6}); // Moving up from a longer row clamps.
}

TEST(BufferSliceTests, CursorDown_WorksAsExpected)
{
    std::vector<Char> chars{
        {.ch = 'H',  .is_deleted = false}, // 0.
        {.ch = 'e',  .is_deleted = false},
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'o',  .is_deleted = false},
        {.ch = ',',  .is_deleted = true }, // Deleted.
        {.ch = ',',  .is_deleted = false},
        {.ch = ',',  .is_deleted = false},
        {.ch = '\n', .is_deleted = false},
        {.ch = 'w',  .is_deleted = false}, // 9.
        {.ch = 'o',  .is_deleted = false},
        {.ch = 'r',  .is_deleted = false},
        {.ch = 'l',  .is_deleted = false},
        {.ch = 'd',  .is_deleted = false},
        {.ch = '!',  .is_deleted = false},
    };
    BufferSlice buf{std::move(chars)};
    verify_cursor<CursorDownAction>(buf, 0, {0, 0}, {1, 0}); // Moving down normally.
    verify_cursor<CursorDownAction>(buf, 9, {1, 0}, {1, 0}); // Moving down from final row is a no-op.
    verify_cursor<CursorDownAction>(buf, 5, {0, 5}, {1, 5}); // Moving down from a deleted character.
    verify_cursor<CursorDownAction>(buf, 8, {0, 7}, {1, 6}); // Moving down from a longer row clamps.
}

TEST(BufferSliceTests, CursorLeft_WorksAsExpected)
{
    std::vector<Char> chars{
        {.ch = 'H', .is_deleted = false}, // 0.
        {.ch = 'e', .is_deleted = false},
        {.ch = 'l', .is_deleted = true },
        {.ch = 'l', .is_deleted = true },
        {.ch = 'o', .is_deleted = false},
    };
    BufferSlice buf{std::move(chars)};
    verify_cursor<CursorLeftAction>(buf, 1, {0, 1}, {0, 0}); // Moving left normally.
    verify_cursor<CursorLeftAction>(buf, 0, {0, 0}, {0, 0}); // Moving left from origin is a no-op.
    verify_cursor<CursorLeftAction>(buf, 3, {0, 2}, {0, 1}); // Moving left from a deleted character.
    verify_cursor<CursorLeftAction>(buf, 4, {0, 2}, {0, 1}); // Moving left over a deleted character.
}

TEST(BufferSliceTests, CursorRight_WorksAsExpected)
{
    std::vector<Char> chars{
        {.ch = 'H', .is_deleted = false}, // 0.
        {.ch = 'e', .is_deleted = false},
        {.ch = 'l', .is_deleted = true },
        {.ch = 'l', .is_deleted = true },
        {.ch = 'o', .is_deleted = false},
    };
    BufferSlice buf{std::move(chars)};
    verify_cursor<CursorRightAction>(buf, 0, {0, 0}, {0, 1}); // Moving right normally.
    verify_cursor<CursorRightAction>(buf, 5, {0, 3}, {0, 3}); // Moving right from `EOF` is a no-op.
    verify_cursor<CursorRightAction>(buf, 2, {0, 2}, {0, 3}); // Moving right from a deleted character.
    verify_cursor<CursorRightAction>(buf, 1, {0, 1}, {0, 2}); // Moving right over a deleted character.
}
