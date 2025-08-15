#include "edit-common/Delta.hpp"

void edit::common::to_json(nlohmann::json &j, const Delta &d)
{
    j = {
        {"d", d.is_delete},
        {"c", d.ch       },
    };
}

void edit::common::from_json(const nlohmann::json &j, Delta &d)
{
    j.at("d").get_to(d.is_delete);
    j.at("c").get_to(d.ch);
}
