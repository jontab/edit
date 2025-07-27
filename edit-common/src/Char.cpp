#include "edit-common/Char.hpp"

bool edit::common::Char::operator==(const edit::common::Char &other) const
{
    return (this->clock == other.clock) && (this->site == other.site);
}

bool edit::common::Char::operator!=(const edit::common::Char &other) const
{
    return !((*this) == other);
}
