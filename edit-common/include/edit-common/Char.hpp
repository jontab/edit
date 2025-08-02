#pragma once

#include <nlohmann/json.hpp>
#include <cstdint>

namespace edit::common
{

struct Char
{
    int parent_clock;
    int parent_site;
    int clock;
    int site;
    std::uint32_t ch; // Unicode.
    bool is_deleted;

    bool operator==(const Char &other) const;
    bool operator!=(const Char &other) const;
};

void to_json(nlohmann::json &j, const Char &ch);
void from_json(const nlohmann::json &j, Char &ch);

} // namespace edit::common
