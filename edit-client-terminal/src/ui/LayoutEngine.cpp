#include "ui/LayoutEngine.hpp"
#include <stdexcept>

using namespace edit::ui;

LayoutEngine::LayoutEngine(unsigned int height, unsigned int width)
    : height_(height)
    , width_(width)
{
}

void LayoutEngine::resize(unsigned int height, unsigned int width)
{
    height_ = height;
    width_ = width;
}

edit::core::Rect<unsigned int> LayoutEngine::buffer_rect() const
{
    if (height_ == 0)
        throw std::runtime_error("Insufficient terminal height");
    return {
        {0,           0     },
        {height_ - 1, width_}
    };
}

edit::core::Rect<unsigned int> LayoutEngine::status_rect() const
{
    if (width_ == 0)
        throw std::runtime_error("Insufficient terminal width");
    return {
        {height_ - 1, 0     },
        {1,           width_}
    };
}
