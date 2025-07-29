#pragma once

#include "IViewBackend.hpp"
#include "Rect.hpp"

namespace edit::ui
{

/**
 * @brief Responsible for drawing a single component.
 */
template <typename T> class IViewComponent
{
  public:
    virtual ~IViewComponent() = default;

    virtual void render(IViewBackend &backend, const T &component, const Rect<unsigned int> &bounds) = 0;
};

} // namespace edit::ui
