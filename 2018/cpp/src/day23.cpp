#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <get_input.hpp>

class Nanobot {
private:
    int xx, yy, zz, rr;
public:
    Nanobot() = default;
    Nanobot(int x, int y, int z, int r)
        : xx{x}, yy{y}, zz{z}, rr{r} { }

    int x() const { return xx; }
    int y() const { return yy; }
    int z() const { return zz; }
    int r() const { return rr; }

    bool is_in_range(const Nanobot& nb) const;
};

int distance(const Nanobot& a, const Nanobot& b)
{
    return std::abs(a.x() - b.x()) +
           std::abs(a.y() - b.y()) +
           std::abs(a.z() - b.z());
}

bool Nanobot::is_in_range(const Nanobot& other) const
{
    return distance(*this, other) <= rr;
}

std::istream& operator>>(std::istream& is, Nanobot& n)
{
    char ch;
    while (is >> ch && ch != '<')
        ;
    int x, y, z;
    is >> x >> ch >> y >> ch >> z;

    while (is >> ch && ch != '=')
        ;
    int r;
    is >> r;
    Nanobot nn { x, y, z, r };
    n = nn;
    return is;
}

auto parse_input(const std::string& input)
{
    std::vector<Nanobot> vnb;

    std::istringstream iss { input };

    for (Nanobot nb; iss >> nb; )
        vnb.emplace_back(nb);

    return vnb;
}

int in_range_of_strongest(const std::vector<Nanobot>& vnb)
{
    auto it = std::max_element(std::begin(vnb), std::end(vnb),
            [](const auto& a, const auto& b) { return a.r() < b.r(); });

    return std::count_if(std::begin(vnb), std::end(vnb),
            [&it](const auto& nb) { return it->is_in_range(nb); });
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 23 - Experimental Emergency Teleportation\n";

    auto input = utils::get_input_string(argc, argv, "23");
    auto vnb = parse_input(input);

    auto part1 = in_range_of_strongest(vnb);
    std::cout << "Part 1: " << part1 << '\n';
}

