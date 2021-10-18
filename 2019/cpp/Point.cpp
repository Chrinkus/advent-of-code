#include "Point.hpp"

#include <cstdlib>
#include <sstream>

int Point::manhatten_distance(const Point& p) const noexcept
{
	return std::abs(xx - p.x()) + std::abs(yy - p.y());
}

std::string Point::to_string() const
{
    std::ostringstream oss;
    oss << xx << ',' << yy;
    return oss.str();
}

bool operator==(const Point& a, const Point& b) noexcept
{
	return a.x() == b.x() && a.y() == b.y();
}

bool operator!=(const Point& a, const Point& b) noexcept
{
	return !(a == b);
}

bool operator<(const Point& a, const Point& b) noexcept
{
	return a.x() < b.x() ||
		(a.x() == b.x() && a.y() < b.y());
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    return os << p.x() << ',' << p.y();
}
