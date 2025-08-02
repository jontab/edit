#include "edit-common/Char.hpp"

bool edit::common::Char::operator==(const edit::common::Char &other) const
{
    return (this->clock == other.clock) && (this->site == other.site);
}

bool edit::common::Char::operator!=(const edit::common::Char &other) const
{
    return !((*this) == other);
}

void edit::common::to_json(nlohmann::json &j, const Char &ch)
{
    j = {
        {"pc", ch.parent_clock},
        {"ps", ch.parent_site },
        {"c",  ch.clock       },
        {"s",  ch.site        },
        {"ch", ch.ch          },
        {"d",  ch.is_deleted  }
    };
}

void edit::common::from_json(const nlohmann::json &j, Char &ch)
{
    j.at("pc").get_to(ch.parent_clock);
    j.at("ps").get_to(ch.parent_site);
    j.at("c").get_to(ch.clock);
    j.at("s").get_to(ch.site);
    j.at("ch").get_to(ch.ch);
    j.at("d").get_to(ch.is_deleted);
}
