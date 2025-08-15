#pragma once

#include "core/Mode.hpp"
#include "state/BufferSlice.hpp"
#include <edit-common/Buffer.hpp>

namespace edit::ui
{

/**
 * @brief `BufferView` needs more than is owned by `BufferSlice` to render. Instead of tightly coupling `BufferView` to
 * `EditorStore`, lets inject state via a "snapshot."
 */
struct BufferViewSnapshot
{
    const state::BufferSlice &slice;
    std::size_t cursor_y;
    std::size_t cursor_x;

    // The following is not owned by `BufferSlice`.
    core::Mode current_mode;
};

}; // namespace edit::ui
