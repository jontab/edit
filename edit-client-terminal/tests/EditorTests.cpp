#include "Editor.hpp"
#include <gtest/gtest.h>

class MockView_QuitsImmediately : public edit::IView
{
  public:
    std::optional<edit::Action> poll() override
    {
        return edit::Action{edit::Quit{}};
    }

    void render(const edit::BufferComponent &buffer, const edit::StatusComponent &status) override
    {
        (void)buffer;
        (void)status;
    }
};

TEST(EditorTests, EditorQuits_WhenQuitPublished)
{
    edit::Editor editor{std::make_unique<MockView_QuitsImmediately>()};
    editor.run();
}
