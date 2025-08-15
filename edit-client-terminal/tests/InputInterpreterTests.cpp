#include "InputInterpreter.hpp"
#include <gtest/gtest.h>

using namespace edit;
using namespace edit::core;
using namespace edit::state;

// TEST(InputInterpreterTests, FromNormalMode_CorrectActions)
// {
//     // Arrange.
//     edit::core::Bus<edit::core::Action> ab;
//     edit::core::Bus<edit::core::Event> eb;
//     EditorStore store{ab, eb};
//     InputInterpreter{ab, eb, store};

//     bool was_change_mode_insert_called = false;
//     bool was_change_mode_command_called = false;
//     bool was_cursor_down_called = false;
//     bool was_cursor_up_called = false;
//     bool was_cursor_left_called = false;
//     bool was_cursor_right_called = false;
//     bool was_delete_called = false;

//     ab.on<actions::ChangeMode>([&](const auto &a) {
//         if (a.to == Mode::InsertMode)
//         {
//             was_change_mode_insert_called = true;
//         }
//         else if (a.to == Mode::CommandMode)
//         {
//             was_change_mode_command_called = true;
//         }
//     });
//     ab.on<actions::CursorDown>([&](const auto &) { was_cursor_down_called = true; });
//     ab.on<actions::CursorUp>([&](const auto &) { was_cursor_up_called = true; });
//     ab.on<actions::CursorLeft>([&](const auto &) { was_cursor_left_called = true; });
//     ab.on<actions::CursorRight>([&](const auto &) { was_cursor_right_called = true; });
//     ab.on<actions::Delete>([&](const auto &) { was_delete_called = true; });

//     // Act.
//     eb.post(events::KeyPressed{'i'});
//     eb.publish();
//     ab.post(actions::Escape{});
//     ab.publish();
//     eb.post(events::KeyPressed{':'});
//     eb.publish();
//     ab.post(actions::Escape{});
//     ab.publish();
//     eb.post(events::KeyPressed{'j'});
//     eb.post(events::KeyPressed{'k'});
//     eb.post(events::KeyPressed{'h'});
//     eb.post(events::KeyPressed{'l'});
//     eb.post(events::KeyPressed{'x'});
//     eb.publish(); // Queues a bunch of `core::Action`.
//     ab.publish(); // Execute.

//     // Assert.
//     ASSERT_TRUE(was_change_mode_insert_called);
//     ASSERT_TRUE(was_change_mode_command_called);
//     ASSERT_TRUE(was_cursor_down_called);
//     ASSERT_TRUE(was_cursor_up_called);
//     ASSERT_TRUE(was_cursor_left_called);
//     ASSERT_TRUE(was_cursor_right_called);
//     ASSERT_TRUE(was_delete_called);
// }

// TEST(InputInterpreterTests, FromInsertMode_CorrectActions)
// {
//     // Arrange.
//     edit::core::Bus<edit::core::Action> ab;
//     edit::core::Bus<edit::core::Event> eb;
//     EditorStore store{ab, eb};
//     InputInterpreter{ab, eb, store};

//     bool was_insert_called = false;
//     bool was_cursor_right_called = false;

//     ab.on<actions::Insert>([&](const auto &a) {
//         if (a.ch == 'a')
//             was_insert_called = true;
//     });
//     ab.on<actions::CursorRight>([&](const auto &) { was_cursor_right_called = true; });

//     // Act.
//     ab.post(actions::ChangeMode{Mode::InsertMode});
//     ab.publish();
//     eb.post(events::KeyPressed{'a'});
//     eb.publish(); // Queues `actions::Insert` and `actions::CursorRight`.
//     ab.publish(); // Execute.

//     // Assert.
//     ASSERT_TRUE(was_insert_called);
//     ASSERT_TRUE(was_cursor_right_called);
// }
