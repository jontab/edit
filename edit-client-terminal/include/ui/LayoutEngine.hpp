#pragma once

#include "core/Rect.hpp"

namespace edit::ui
{

/**
 * @brief Responsible for partitioning the display.
 */
class LayoutEngine
{
    unsigned int height_;
    unsigned int width_;

  public:
    LayoutEngine(unsigned int height, unsigned int width);

    void resize(unsigned int height, unsigned int width);
    core::Rect<unsigned int> buffer_rect() const;
    core::Rect<unsigned int> status_rect() const;
};

} // namespace edit::ui
