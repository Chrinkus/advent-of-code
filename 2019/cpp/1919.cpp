#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Intcode_vm.hpp"
#include "Point.hpp"

int deploy_drone(const std::string& input, const Point& p)
{
    Intcode_vm vm {input};
    std::stringstream iss;
    std::stringstream oss;
    iss << p.x() << '\n' << p.y() << '\n';
    vm.run(iss, oss);
    int res;
    oss >> res;
    return res;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 19\n"
              << "Tractor Beam\n";

    constexpr int max_w = 50;
    constexpr int max_h = 50;

    std::string input;
    std::cin >> input;

    std::vector<std::string> vs (max_h);
    for (auto& s : vs)
        s.resize(max_w, '.');


	auto part1 = 0;
    for (auto i = 0, pull = 0; i < vs.size(); ++i) {
        bool pulling = false;
        for (auto j = pull; j < vs[i].size(); ++j) {
            int res = deploy_drone(input, Point{j, i});
            if (res) {
                if (!pulling) {
                    pulling = true;
                    pull = j;
                }
                ++part1;
                vs[i][j] = '#';
            } else if (pulling) {
                break;
            }
        }
    }

    /*
    for (const auto& s : vs)
        std::cout << s << '\n';
    */

	auto part2 = 0;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
