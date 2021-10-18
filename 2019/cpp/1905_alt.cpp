#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Intcode_vm.hpp"

int main()
{
    std::cout << "Advent of Code 2019 - Day 5 Alternate\n"
              << "Sunny with a Chance of Asteroids\n";

    std::string input;
    std::cin >> input;

    Intcode_vm vm1 {input};
    Intcode_vm vm2 {input};
    std::stringstream io;

    io << 1 << '\n';
    vm1.run(io, io);
	int part1;
    while (io >> part1)
        std::cout << part1 << '\n';

    io.clear();
    io << 5 << '\n';
    vm2.run(io, io);
	int part2;
    while (io >> part2)
        std::cout << part2 << '\n';

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
