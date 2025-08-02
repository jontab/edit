#pragma once

#include <edit-common/Buffer.hpp>
#include <set>

struct BufferSession
{
    edit::common::Buffer buffer;
    std::set<void *> users;
};
