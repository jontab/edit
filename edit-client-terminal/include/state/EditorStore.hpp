#pragma once

#include "BufferSlice.hpp"
#include "ModeSlice.hpp"
#include "StatusSlice.hpp"
#include "core/Dispatcher.hpp"

namespace edit
{

class EditorStore
{
    Dispatcher &dispatcher_;
    BufferSlice buffer_;
    ModeSlice mode_;
    StatusSlice status_;

  public:
    EditorStore(Dispatcher &dispatcher);

    const BufferSlice &buffer() const;
    const ModeSlice &mode() const;
    const StatusSlice &status() const;

  private:
    void reduce(const InsertAction &a);
    void reduce(const DeleteAction &a);
    void reduce(const BackspaceAction &a);
    void reduce(const CursorUpAction &a);
    void reduce(const CursorDownAction &a);
    void reduce(const CursorLeftAction &a);
    void reduce(const CursorRightAction &a);
};

} // namespace edit
