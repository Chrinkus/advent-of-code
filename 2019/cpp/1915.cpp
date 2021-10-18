#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

#include "Intcode_vm.hpp"
#include "Point.hpp"

class Droid {
    enum class Dir { north=1, south, west, east };
    enum class Status { wall, moved, goal };
public:
    Droid() = delete;
    Droid(const std::string& intcode)
        : brain{intcode} { }

    Point location() const { return loc; }

private:
    Intcode_vm brain;
    Point loc;
    std::stringstream input;
    std::stringstream output;
};

std::unordered_map<std::string,char> feel_around_in_the_dark(Droid& droid)
{
    std::unordered_map<std::string,char> umap;

    return umap;
}

int shortest_path(const std::unordered_map<std::string,char>& um)
{
    return um.size();
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 15\n"
              << "Oxygen System\n";

    std::string input;
    std::cin >> input;

    Droid droid {input};
    std::unordered_map<std::string,char> umap = feel_around_in_the_dark(droid);

	auto part1 = shortest_path(umap);
	auto part2 = 0;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
