#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <array>

#include <get_input.hpp>

struct Claim {
    int id = 0;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

constexpr size_t FAB_WIDTH = 1000;

class Fabric {
public:
    Fabric()
    {
        std::for_each(std::begin(fabric), std::end(fabric),
                [](auto& arr) { arr.fill(0); });
    }

    void stake_claim(const Claim& c);

    int count_overlaps() const;
    int find_intact_id(const std::vector<Claim>& claims) const;

private:
    std::array<std::array<int,FAB_WIDTH>,FAB_WIDTH> fabric;
};

void Fabric::stake_claim(const Claim& c)
{
    for (int i = c.y; i < c.y + c.h; ++i)
        for (int j = c.x; j < c.x + c.w; ++j)
            ++fabric[i][j];
}

int Fabric::count_overlaps() const
{
    int count = 0;
    for (const auto& ai : fabric)
        for (const auto i : ai)
            if (i > 1)
                ++count;
    return count;
}

int Fabric::find_intact_id(const std::vector<Claim>& claims) const
{
    for (const auto& c : claims) {
        bool intact = true;
        for (int i = c.y; intact && i < c.y + c.h; ++i)
            for (int j = c.x; intact && j < c.x + c.w; ++j)
                if (fabric[i][j] != 1)
                    intact = false;
        if (intact)
            return c.id;
    }

    return 0;       // no swaths are intact
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

auto parse_claims(const std::vector<std::string>& vs)
{
    std::vector<Claim> vc;
    vc.reserve(vs.size());

    const std::regex claim_pat {R"(^#(\d+)\s@\s(\d+),(\d+):\s(\d+)x(\d+))"};
    std::smatch matches;

    std::for_each(std::begin(vs), std::end(vs),
            [&](const std::string& line) {
                std::regex_match(line, matches, claim_pat);
                auto it = ++matches.begin();
                Claim claim {};
                claim.id = std::stoi(*it++);
                claim.x  = std::stoi(*it++);
                claim.y  = std::stoi(*it++);
                claim.w  = std::stoi(*it++);
                claim.h  = std::stoi(*it);
                vc.push_back(claim);
            });

    return vc;
}

auto map_claims(const std::vector<Claim>& vc)
{
    Fabric fabric;

    std::for_each(std::begin(vc), std::end(vc),
            [&fabric](const Claim& c) { fabric.stake_claim(c); });

    return fabric;
}

int main()
{
    auto input = utils::get_input_lines();

    auto claims = parse_claims(input);
    auto fabric = map_claims(claims);

    auto part1 = fabric.count_overlaps();
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = fabric.find_intact_id(claims);
    std::cout << "Part 2: " << part2 << '\n';
}
