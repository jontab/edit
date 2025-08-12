#include "state/BufferState.hpp"

using namespace edit::common;
using namespace edit::state;

BufferState::BufferState()
    : buffer()
    , lines()
    , cursor()
    , site(rand())
{
}

BufferState::BufferState(const std::vector<Char> &chars)
    : buffer(chars)
    , lines()
    , cursor()
    , site(rand())
{
}

BufferState::BufferState(std::vector<common::Char> &&chars)
    : buffer(std::move(chars))
    , lines()
    , cursor()
    , site(rand())
{
}

std::vector<Char>::const_iterator BufferState::line_begin(std::size_t y) const
{
    if (y < lines.size())
    {
        return buffer.begin() + lines[y].begin;
    }
    else
    {
        return buffer.end();
    }
}

std::vector<Char>::const_iterator BufferState::line_end(std::size_t y) const
{
    if (y < lines.size())
    {
        return buffer.begin() + lines[y].end;
    }
    else
    {
        return buffer.end();
    }
}
