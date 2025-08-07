#pragma once

#include <edit-common/Buffer.hpp>

namespace edit
{

struct BufferState
{
    struct Line
    {
        std::size_t begin;
        std::size_t end;
    };

    common::Buffer buffer;
    std::vector<Line> lines;
    std::size_t cursor;
    int site;

    inline BufferState()
        : buffer()
        , lines()
        , cursor()
        , site(rand())
    {
    }

    inline BufferState(const std::vector<common::Char> &chars)
        : buffer(chars)
        , lines()
        , cursor()
        , site(rand())
    {
    }

    inline BufferState(std::vector<common::Char> &&chars)
        : buffer(std::move(chars))
        , lines()
        , cursor()
        , site(rand())
    {
    }
};

} // namespace edit
