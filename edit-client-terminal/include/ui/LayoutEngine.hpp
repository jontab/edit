#pragma once

#include "Rect.hpp"

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
    Rect<unsigned int> buffer_rect() const;
    Rect<unsigned int> status_rect() const;
};

} // namespace edit::ui
