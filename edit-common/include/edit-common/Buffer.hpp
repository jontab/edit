#pragma once

#include "edit-common/Char.hpp"
#include <vector>

namespace edit::common
{

class Buffer
{
    std::vector<Char> data;

  public:
    Buffer();
    Buffer(const std::vector<Char> &data);
    Buffer(std::vector<Char> &&data);

    bool insert(const Char &ch);
    bool remove(const Char &ch);

    std::vector<Char>::iterator begin();
    std::vector<Char>::iterator end();
    std::vector<Char>::const_iterator begin() const;
    std::vector<Char>::const_iterator end() const;
};

int find(const std::vector<Char> &chars, const Char &ch);
int find_sorted_position(const std::vector<Char> &chars, const Char &ch);

} // namespace edit::common
