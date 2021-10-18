#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "Point.hpp"

class Slope {
public:
    Slope() = delete;
    Slope(int rise, int run);

    int rise() const { return ri; }
    int run()  const { return ru; }
private:
    int ri;
    int ru;
};

Slope::Slope(int rise, int run)
    : ri{rise}, ru{run}
{
    if (ri == 0) {
        ru = ru > 0 ? 1 : -1;
    } else if (ru == 0) {
        ri = ri > 0 ? 1 : -1;
    } else {
        auto cd = std::gcd(rise, run);
        ri /= cd;
        ru /= cd;
    }   
}

bool operator==(const Slope& a, const Slope& b)
{
    return (a.rise() == b.rise() && a.run() == b.run());
}

bool operator!=(const Slope& a, const Slope& b)
{
    return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const Slope& s)
{
    return os << s.rise() << '/' << s.run();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Slope operator-(const Point& a, const Point& b)
{
    return Slope{a.y() - b.y(), a.x() - b.x()};
}

struct Asteroid {
    Point loc;
    size_t sees;
};

bool operator==(const Asteroid& a, const Asteroid& b)
{
    return a.loc == b.loc;
}

bool operator!=(const Asteroid& a, const Asteroid& b)
{
    return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const Asteroid& a)
{
    return os << "Location: " << a.loc << " Sees: " << a.sees;
}

/*
enum class Quadrant { I, II, III, IV };

Quadrant get_quadrant(const double x, const double y)
    // arguments must be non-zero
{
    if (x > 0) {
        if (y < 0)
            return Quadrant::I;
        else
            return Quadrant::II;
    } else {
        if (y > 0)
            return Quadrant::III;
        else
            return Quadrant::IV;
    }
}

double get_shift(const double x, const double y)
{
    Quadrant q = get_quadrant(x, y);
    switch (q) {
        case Quadrant::I:
        case Quadrant::II:
            return 90;
        case Quadrant::III:
        case Quadrant::IV:
            return 270;
    }
}
*/

struct Vector {
    Point term;
    double mag;
    double dir;
    Vector(const Point& i, const Point& t);
};

Vector::Vector(const Point& i, const Point& t)
    : term{t}
{
    double dx = t.x() - i.x();
    double dy = t.y() - i.y();
    mag = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    double ang = std::atan2(dy, dx) * 180.0 / M_PI;
    dir = ang < -90 ? ang + 450 : ang + 90;
    //dir = std::atan2(dy, dx) * 180.0 / M_PI;
}

bool operator<(const Vector& a, const Vector& b)
{
    return a.dir < b.dir ||
        (a.dir == b.dir && a.mag < b.mag);
}

bool operator==(const Vector& a, const Vector& b)
{
    return a.dir == b.dir && a.mag == b.mag;
}

bool operator!=(const Vector& a, const Vector& b)
{
    return !(a == b);
}

std::vector<Vector> spiral_sort(const std::vector<Vector>& vv)
{
    std::vector<Vector> spiral;

    while (spiral.size() < vv.size()) {
        double prev = 360;
        for (const auto& v : vv) {
            if (v.dir == prev ||
                    std::find(std::begin(spiral),
                        std::end(spiral), v) != std::end(spiral))
                continue;
            prev = v.dir;
            spiral.push_back(v);
        }
    }
    return spiral;
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    return os << '[' << v.term << "] direction: " << v.dir
              << " magnitude: " << v.mag;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 10\n"
              << "Monitoring Station\n";

	std::vector<std::string> space_field;
	for (std::string s; std::getline(std::cin, s); )
		space_field.push_back(s);

	std::cout << "Input size: " << space_field.size() << '\n'
              << "  * first: " << space_field.front() << '\n'
              << "  * last : " << space_field.back() << '\n';

    constexpr char ASTEROID = '#';

    std::vector<Asteroid> va;
    for (auto i = 0; i < space_field.size(); ++i)
        for (auto j = 0; j < space_field[i].size(); ++j)
            if (space_field[i][j] == ASTEROID)
                va.emplace_back(Asteroid{Point{j,i}, 0});
    std::cout << "Num asteroids: " << va.size() << '\n';

    for (Asteroid& a : va) {
        std::vector<Slope> vs;
        for (const Asteroid& b : va) {
            if (a == b)
                continue;
            Slope sl = b.loc - a.loc;
            if (std::find(std::begin(vs), std::end(vs), sl) == std::end(vs))
                vs.push_back(sl);
        }
        a.sees = vs.size();
    }

	auto it = std::max_element(std::begin(va), std::end(va),
            [](const auto& a, const auto& b) {
                return a.sees < b.sees;
            });
    auto base = *it;
    auto part1 = base.sees;

    va.erase(it);           // remove base from asteroid(target) field
    std::vector<Vector> vv;
    for (const auto a : va)
        vv.emplace_back(Vector{base.loc, a.loc});
    std::sort(std::begin(vv), std::end(vv));
    /*
    for (const auto& v : vv)
        std::cout << v << '\n';
    */
    std::vector<Vector> spiral = spiral_sort(vv);

    auto num_200 = spiral[199];
    std::cout << "Base at: " << base.loc << '\n';
	auto part2 = num_200.term.x() * 100 + num_200.term.y();

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
