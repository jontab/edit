#pragma once

#include <edit-common/Buffer.hpp>

namespace edit::state
{

struct BufferState
{
    struct Line
    {
        std::size_t begin;
        std::size_t end;
    };

    edit::common::Buffer buffer;
    std::vector<Line> lines;
    std::size_t cursor;
    int site;

    BufferState();
    BufferState(const std::vector<common::Char> &chars);
    BufferState(std::vector<common::Char> &&chars);

    std::vector<common::Char>::const_iterator line_begin(std::size_t y) const;
    std::vector<common::Char>::const_iterator line_end(std::size_t y) const;
};

} // namespace edit::state
