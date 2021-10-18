#include <iostream>
#include <string>
#include <sstream>

#include "Intcode_vm.hpp"

int main()
{
    std::cout << "Advent of Code 2019 - Day 9\n"
              << "Sensor Boost\n";

    std::string input;
    std::cin >> input;

    Intcode_vm vm1 {input};
    std::stringstream ss;
    ss << 1 << '\n';
    vm1.run(ss, ss);

    int64_t res;
    while (ss >> res)
        std::cout << res << '\n';
	auto part1 = res;

    Intcode_vm vm2 {input};
    std::stringstream ss2;
    ss2 << 2 << '\n';
    vm2.run(ss2, ss2);
    while (ss2 >> res)
        std::cout << res << '\n';
	auto part2 = res;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
