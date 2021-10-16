#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

#include <get_input.hpp>

constexpr size_t FAB_WIDTH = 1000;

using Fabric = std::array<std::array<int,FAB_WIDTH>,FAB_WIDTH>;

struct Claim {
    int id = 0;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

std::istream& operator>>(std::istream& is, Claim& claim)
{
    char c;
    int id, x, y, w, h;
    //    #          @         ,         :         x
    is >> c >> id >> c >> x >> c >> y >> c >> w >> c >> h;
    claim = Claim{ id, x, y, w, h };

    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

auto map_claims(const std::vector<Claim>& vc)
{
    Fabric fabric;
    std::for_each(std::begin(fabric), std::end(fabric),
            [](auto& arr) { arr.fill(0); });

    std::for_each(std::begin(vc), std::end(vc),
            [&fabric](const Claim& c) {
                for (int i = c.y; i < c.y + c.h; ++i)
                    for (int j = c.x; j < c.x + c.w; ++j)
                        ++fabric[i][j];
            });

    return fabric;
}

int count_overlaps(const Fabric& aai)
{
    int count = 0;
    for (const auto& ai : aai)
        for (const auto i : ai)
            if (i > 1)
                ++count;
    return count;
}

int find_intact_id(const std::vector<Claim>& vc, const Fabric& fab)
{
    for (const auto& c : vc) {
        bool intact = true;
        for (int i = c.y; intact && i < c.y + c.h; ++i)
            for (int j = c.x; intact && j < c.x + c.w; ++j)
                if (fab[i][j] != 1)
                    intact = false;
        if (intact)
            return c.id;
    }

    return 0;       // no swaths are intact
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 3 - No Matter How You Slice It\n";
    auto claims = utils::get_input_values<Claim>(argc, argv, "03");
    auto fabric = map_claims(claims);

    auto part1 = count_overlaps(fabric);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = find_intact_id(claims, fabric);
    std::cout << "Part 2: " << part2 << '\n';
}
