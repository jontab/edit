#pragma once

#include "core/Mode.hpp"
#include "state/StatusSlice.hpp"
#include <cstddef>
#include <string>

namespace edit::ui
{

/**
 * @brief `StatusView` needs more data than the `StatusSlice` can provide. Without coupling `StatusView` to, e.g., the
 * `EditorStore`, we can take a "snapshot" of the data we need the `StatusView` to render and pass that instead.
 */
struct StatusViewSnapshot
{
    const edit::state::StatusSlice &slice;

    // The following is not owned by `StatusSlice` and needs to be passed via context.
    edit::core::Mode current_mode;
    std::size_t buffer_cursor_index;
    std::size_t buffer_cursor_y;
    std::size_t buffer_cursor_x;
};

} // namespace edit::ui
