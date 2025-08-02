#pragma once

#include <edit-common/Buffer.hpp>
#include <optional>
#include <string>

class IDatabase
{
  public:
    virtual ~IDatabase() = default;

    virtual std::optional<edit::common::Buffer> load(const std::string &docid) = 0;
    virtual void save(const std::string &docid, const edit::common::Buffer &buffer) = 0;
};
