#pragma once

#include "BufferSlice.hpp"
#include "ModeSlice.hpp"
#include "StatusSlice.hpp"
#include "core/Dispatcher.hpp"

namespace edit::state
{

class EditorStore
{
    edit::core::Dispatcher &dispatcher_;
    BufferSlice buffer_;
    ModeSlice mode_;
    StatusSlice status_;

  public:
    EditorStore(edit::core::Dispatcher &dispatcher);

    const BufferSlice &buffer() const;
    const ModeSlice &mode() const;
    const StatusSlice &status() const;

  private:
    void reduce(const edit::core::InsertAction &a);
    void reduce(const edit::core::DeleteAction &a);
    void reduce(const edit::core::BackspaceAction &a);
    void reduce(const edit::core::CursorUpAction &a);
    void reduce(const edit::core::CursorDownAction &a);
    void reduce(const edit::core::CursorLeftAction &a);
    void reduce(const edit::core::CursorRightAction &a);
    void reduce(const edit::core::ChangeStatusAction &a);
};

} // namespace edit::state
