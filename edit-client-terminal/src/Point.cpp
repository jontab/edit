#include "Point.hpp"

edit::Point::Point(decltype(y) y, decltype(x) x) : y(y), x(x)
{
}

bool edit::Point::operator==(const Point &other) const
{
    return (y == other.y) && (x == other.x);
}

bool edit::Point::operator!=(const Point &other) const
{
    return !((*this) == other);
}
