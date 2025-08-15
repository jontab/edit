#include "CommandInterpreter.hpp"
#include "core/ActionTypes.hpp"
#include <gtest/gtest.h>

using namespace edit;
using namespace edit::core;

TEST(CommandInterpreterTests, EmitsQuitAction)
{
    // Arrange.
    core::Bus<core::Action> ab;
    core::Bus<core::Event> eb;
    CommandInterpreter interpreter{ab, eb};

    bool was_quit_called = false;

    ab.on<actions::Quit>([&](const auto &a) { was_quit_called = true; });

    // Act.
    eb.post(events::CommandEntered{":quit"});
    eb.publish(); // Queues `actions::Quit`.
    ab.publish(); // Execute.

    // Assert.
    ASSERT_TRUE(was_quit_called);
}

TEST(CommandInterpreterTests, SwitchesToNormalMode)
{
    // Arrange.
    core::Bus<core::Action> ab;
    core::Bus<core::Event> eb;
    CommandInterpreter interpreter{ab, eb};

    bool was_change_mode_normal_called = false;

    ab.on<actions::ChangeMode>([&](const auto &a) {
        if (a.to == Mode::NormalMode)
            was_change_mode_normal_called = true;
    });

    // Act.
    eb.post(events::CommandEntered{":apple"});
    eb.publish(); // Queues `actions::ChangeMode`.
    ab.publish(); // Execute.

    // Assert.
    ASSERT_TRUE(was_change_mode_normal_called);
}
