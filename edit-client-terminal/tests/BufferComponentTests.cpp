#include "components/BufferComponent.hpp"
#include <gtest/gtest.h>

using ActionBus = edit::ActionBus;
using EventBus = edit::EventBus;
using BufferComponent = edit::BufferComponent;
using Point = edit::ui::Point<std::size_t>;

template <typename ActionType>
void verify_cursor(ActionBus &bus, BufferComponent &buffer, std::size_t index, const Point &from, const Point &to)
{
    buffer.set_cursor_index(index);
    EXPECT_EQ(buffer.get_cursor_position(), from);
    bus.publish(edit::Action{ActionType{}});
    EXPECT_EQ(buffer.get_cursor_position(), to);
}

TEST(BufferComponentTests, Site_IsRandom)
{
    srand(1337);
    ActionBus bus;
    EventBus evbus;
    BufferComponent a{bus, evbus};
    BufferComponent b{bus, evbus};
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
    ActionBus bus;
    EventBus evbus;
    BufferComponent buffer{bus, evbus, std::move(chars)};
    verify_cursor<edit::CursorUp>(bus, buffer, 7, {1, 0}, {0, 0});  // Moving up normally.
    verify_cursor<edit::CursorUp>(bus, buffer, 0, {0, 0}, {0, 0});  // Moving up from first row is a no-op.
    verify_cursor<edit::CursorUp>(bus, buffer, 9, {1, 2}, {0, 2});  // Moving up from a deleted character.
    verify_cursor<edit::CursorUp>(bus, buffer, 15, {1, 7}, {0, 6}); // Moving up from a longer row clamps.
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
    ActionBus bus;
    EventBus evbus;
    BufferComponent buffer{bus, evbus, std::move(chars)};
    verify_cursor<edit::CursorDown>(bus, buffer, 0, {0, 0}, {1, 0}); // Moving down normally.
    verify_cursor<edit::CursorDown>(bus, buffer, 9, {1, 0}, {1, 0}); // Moving down from final row is a no-op.
    verify_cursor<edit::CursorDown>(bus, buffer, 5, {0, 5}, {1, 5}); // Moving down from a deleted character.
    verify_cursor<edit::CursorDown>(bus, buffer, 8, {0, 7}, {1, 6}); // Moving down from a longer row clamps.
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
    ActionBus bus;
    EventBus evbus;
    BufferComponent buffer{bus, evbus, std::move(chars)};
    verify_cursor<edit::CursorLeft>(bus, buffer, 1, {0, 1}, {0, 0}); // Moving left normally.
    verify_cursor<edit::CursorLeft>(bus, buffer, 0, {0, 0}, {0, 0}); // Moving left from origin is a no-op.
    verify_cursor<edit::CursorLeft>(bus, buffer, 3, {0, 2}, {0, 1}); // Moving left from a deleted character.
    verify_cursor<edit::CursorLeft>(bus, buffer, 4, {0, 2}, {0, 1}); // Moving left over a deleted character.
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
    ActionBus bus;
    EventBus evbus;
    BufferComponent buffer{bus, evbus, std::move(chars)};
    verify_cursor<edit::CursorRight>(bus, buffer, 0, {0, 0}, {0, 1}); // Moving right normally.
    verify_cursor<edit::CursorRight>(bus, buffer, 5, {0, 3}, {0, 3}); // Moving right from `EOF` is a no-op.
    verify_cursor<edit::CursorRight>(bus, buffer, 2, {0, 2}, {0, 3}); // Moving right from a deleted character.
    verify_cursor<edit::CursorRight>(bus, buffer, 1, {0, 1}, {0, 2}); // Moving right over a deleted character.
}
