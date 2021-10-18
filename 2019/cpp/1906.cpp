#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "utils.hpp"

using Object_map = std::unordered_map<std::string,std::string>;

const std::string SENTINEL = "COM";
const std::string START = "YOU";
const std::string GOAL = "SAN";

int count_orbits(const Object_map& om)
{
    int count = 0;
    for (const auto& o : om)
        for (auto s = o.first; s != SENTINEL; s = om.at(s))
            ++count;
    return count;
}

int count_orbits_from_to(const Object_map& om, const std::string& from,
        const std::string& to)
{
    int count = 0;
    for (auto s = om.at(from); s != to; s = om.at(s))
        ++count;
    return count;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
std::vector<std::string>
trace_route(const Object_map& om, const std::string& obj)
{
    std::vector<std::string> route;
    for (auto s = om.at(obj); s != SENTINEL; s = om.at(s))
        route.push_back(s);
    return route;
}

int count_transfers(const Object_map& om)
{
    std::vector<std::string> you_map = trace_route(om, START);
    std::vector<std::string> san_map = trace_route(om, GOAL);

    auto res = std::find_first_of(std::begin(you_map), std::end(you_map),
                                  std::begin(san_map), std::end(san_map));

    if (res == std::end(you_map))
        return -1;

    return count_orbits_from_to(om, START, *res) +
           count_orbits_from_to(om, GOAL, *res);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    std::cout << "Advent of Code 2019 - Day 6\n"
              << "Universal Orbit Map\n";

	std::vector<std::string> vs;
	for (std::string orbit; std::cin >> orbit; )
		vs.push_back(orbit);

	std::cout << "Input size: " << vs.size() << '\n'
              << "  * first: " << vs.front() << '\n'
              << "  * last : " << vs.back() << '\n';

    Object_map obj_map;
    for (const auto& s : vs) {
        auto v = split(s, ')');
        obj_map[v[1]] = v[0];
    }

	int part1 = count_orbits(obj_map);
	int part2 = count_transfers(obj_map);

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
