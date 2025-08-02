#pragma once

#include "edit-common/Char.hpp"
#include <vector>

namespace edit::common
{

class Buffer
{
    std::vector<Char> data_;
    int clock_;

  public:
    Buffer();
    Buffer(const std::vector<Char> &data);
    Buffer(std::vector<Char> &&data);

    bool insert(const Char &ch);
    bool remove(const Char &ch);

    int clock() const;
    std::size_t size() const;
    std::vector<Char>::iterator begin();
    std::vector<Char>::iterator end();
    std::vector<Char>::const_iterator begin() const;
    std::vector<Char>::const_iterator end() const;
    Char &operator[](std::size_t index);
    const Char &operator[](std::size_t index) const;

    friend void to_json(nlohmann::json &j, const Buffer &buffer);
    friend void from_json(const nlohmann::json &j, Buffer &buffer);

};

int find(const std::vector<Char> &chars, const Char &ch);
int find_sorted_position(const std::vector<Char> &chars, const Char &ch);
void to_json(nlohmann::json &j, const Buffer &buffer);
void from_json(const nlohmann::json &j, Buffer &buffer);

} // namespace edit::common
