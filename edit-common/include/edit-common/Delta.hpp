#pragma once

#include "Char.hpp"

namespace edit::common
{

struct Delta
{
    bool is_delete;
    Char ch;
};

void to_json(nlohmann::json &j, const Delta &d);
void from_json(const nlohmann::json &j, Delta &d);

} // namespace edit::common
