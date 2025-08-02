#include "Editor.hpp"
#include <gtest/gtest.h>

// class MockView_QuitsImmediately : public edit::ui::
// {
//   public:
//     void poll(const std::function<void(edit::Action)> &on_action) override
//     {
//         on_action(edit::QuitAction{});
//     }

//     void render(const edit::BufferComponent &buffer, const edit::StatusComponent &status) override
//     {
//         (void)buffer;
//         (void)status;
//     }
// };

// TEST(EditorTests, EditorQuits_WhenQuitPublished)
// {
//     edit::Editor editor{std::make_unique<MockView_QuitsImmediately>()};
//     editor.run();
// }
