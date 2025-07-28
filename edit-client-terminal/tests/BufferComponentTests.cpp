#include "BufferComponent.hpp"
#include <gtest/gtest.h>

void checkpos(const std::pair<std::size_t, std::size_t> &a, const std::pair<std::size_t, std::size_t> &b)
{
    ASSERT_EQ(a.first, b.first);
    ASSERT_EQ(a.second, b.second);
}

TEST(BufferComponentTests, Site_IsRandom)
{
    srand(1337);
    edit::ActionBus bus;
    edit::BufferComponent a{bus};
    edit::BufferComponent b{bus};
    EXPECT_NE(a.site(), b.site());
}

TEST(BufferComponentTests, CursorUp_WorksAsExpected)
{
    std::vector<edit::common::Char> chars{
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
    edit::ActionBus bus;
    edit::BufferComponent buf{bus, std::move(chars)};

    // Moving up normally.
    buf.set_cursor_index(7);
    checkpos(buf.get_cursor_position(), std::make_pair(1, 0));
    bus.publish(edit::Action{edit::CursorUp{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));

    // Moving up from initial row doesn't do anything.
    buf.set_cursor_index(0);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));
    bus.publish(edit::Action{edit::CursorUp{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));

    // Moving up from a deleted character.
    buf.set_cursor_index(9);
    checkpos(buf.get_cursor_position(), std::make_pair(1, 2));
    bus.publish(edit::Action{edit::CursorUp{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 2));

    // Moving up from a longer row clamps the column.
    buf.set_cursor_index(15);
    checkpos(buf.get_cursor_position(), std::make_pair(1, 7));
    bus.publish(edit::Action{edit::CursorUp{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 6));
}

TEST(BufferComponentTests, CursorDown_WorksAsExpected)
{
    std::vector<edit::common::Char> chars{
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
    edit::ActionBus bus;
    edit::BufferComponent buf{bus, std::move(chars)};

    // Moving down normally.
    buf.set_cursor_index(0);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));
    bus.publish(edit::Action{edit::CursorDown{}});
    checkpos(buf.get_cursor_position(), std::make_pair(1, 0));

    // Moving down from final row doesn't do anything.
    buf.set_cursor_index(9);
    checkpos(buf.get_cursor_position(), std::make_pair(1, 0));
    bus.publish(edit::Action{edit::CursorDown{}});
    checkpos(buf.get_cursor_position(), std::make_pair(1, 0));

    // Moving down from a deleted character.
    buf.set_cursor_index(5);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 5));
    bus.publish(edit::Action{edit::CursorDown{}});
    checkpos(buf.get_cursor_position(), std::make_pair(1, 5));

    // Moving down from a longer row clamps the column.
    buf.set_cursor_index(8);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 7));
    bus.publish(edit::Action{edit::CursorDown{}});
    checkpos(buf.get_cursor_position(), std::make_pair(1, 6));
}

TEST(BufferComponentTests, CursorLeft_WorksAsExpected)
{
    std::vector<edit::common::Char> chars{
        {.ch = 'H', .is_deleted = false}, // 0.
        {.ch = 'e', .is_deleted = false},
        {.ch = 'l', .is_deleted = true },
        {.ch = 'l', .is_deleted = true },
        {.ch = 'o', .is_deleted = false},
    };
    edit::ActionBus bus;
    edit::BufferComponent buf{bus, std::move(chars)};

    // Moving left normally.
    buf.set_cursor_index(1);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 1));
    bus.publish(edit::Action{edit::CursorLeft{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));

    // Moving left from origin doesn't do anything.
    buf.set_cursor_index(0);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));
    bus.publish(edit::Action{edit::CursorLeft{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));

    // Moving left from a deleted character.
    buf.set_cursor_index(3);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 2));
    bus.publish(edit::Action{edit::CursorLeft{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 1));

    // Moving left over a deleted character.
    buf.set_cursor_index(4);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 2));
    bus.publish(edit::Action{edit::CursorLeft{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 1));
}

TEST(BufferComponentTests, CursorRight_WorksAsExpected)
{
    std::vector<edit::common::Char> chars{
        {.ch = 'H', .is_deleted = false}, // 0.
        {.ch = 'e', .is_deleted = false},
        {.ch = 'l', .is_deleted = true },
        {.ch = 'l', .is_deleted = true },
        {.ch = 'o', .is_deleted = false},
    };
    edit::ActionBus bus;
    edit::BufferComponent buf{bus, std::move(chars)};

    // Moving right normally.
    buf.set_cursor_index(0);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 0));
    bus.publish(edit::Action{edit::CursorRight{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 1));

    // Moving right from `EOF` doesn't do anything.
    buf.set_cursor_index(5);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 3));
    bus.publish(edit::Action{edit::CursorRight{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 3));

    // Moving right from a deleted character.
    buf.set_cursor_index(2);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 2));
    bus.publish(edit::Action{edit::CursorRight{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 3));

    // Moving right over a deleted character.
    buf.set_cursor_index(1);
    checkpos(buf.get_cursor_position(), std::make_pair(0, 1));
    bus.publish(edit::Action{edit::CursorRight{}});
    checkpos(buf.get_cursor_position(), std::make_pair(0, 2));
}
