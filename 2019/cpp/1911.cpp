#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "Intcode_vm.hpp"
#include "Point.hpp"

enum class Facing { up, right, down, left };    // clockwise rotation

Facing operator++(Facing& f)
{
    f = (f == Facing::left) ? Facing::up
        : static_cast<Facing>(static_cast<int>(f) + 1);
    return f;
}

Facing operator--(Facing& f)
{
    f = (f == Facing::up) ? Facing::left
        : static_cast<Facing>(static_cast<int>(f) - 1);
    return f;
}

class Robot {
public:
    Robot() = default;

    Point location() const { return loc; }

    void turn(int val) { val == 0 ? --f : ++f; }
    void move();
private:
    Facing f = Facing::up;
    Point loc;
};

void Robot::move()
{
    switch (f) {
        case Facing::up:    loc.move(0, -1);    break;
        case Facing::right: loc.move(1, 0);     break;
        case Facing::down:  loc.move(0, 1);     break;
        case Facing::left:  loc.move(-1, 0);    break;
    }
}

struct Panel {
    Point loc;
    char col = '.';
};

int main()
{
    std::cout << "Advent of Code 2019 - Day 11\n"
              << "Space Police\n";

    std::string input;
    std::cin >> input;
    Intcode_vm brain {input};
    std::stringstream iss;
    std::stringstream oss;

    Robot rob;

    std::unordered_map<std::string,char> hull;

    while (!brain.halted()) {
        auto key = rob.location().to_string();
        auto it = hull.find(key);
        if (it == std::end(hull))
            hull[key] = '.';
        auto panel = hull[key] == '.' ? 0 : 1;
        iss << panel << ' ';
        brain.run(iss, oss);
        int64_t col, turn;
        oss >> col >> turn;
        hull[key] = col == 1 ? '#' : '.';
        rob.turn(turn);
        rob.move();
    }

    Intcode_vm brain2 {input};
    Robot rob2;
    std::unordered_map<std::string,Panel> hull2;
    hull2[rob2.location().to_string()] = Panel{rob2.location(), '#'};

    while (!brain2.halted()) {
        auto key = rob2.location().to_string();
        auto it = hull2.find(key);
        if (it == std::end(hull2))
            hull2[key] = Panel{rob2.location(), '.'};
        auto panel = hull2[key].col == '.' ? 0 : 1;
        iss << panel << ' ';
        brain2.run(iss, oss);
        int64_t col, turn;
        oss >> col >> turn;
        hull2[key].col = col == 1 ? '#' : '.';
        rob2.turn(turn);
        rob2.move();
    }

    const auto width = 48;
    const auto height = 8;
    std::vector<std::string> vs (height);
    for (auto& s : vs)
        s = std::string(width, '.');
    auto xoffset = width / 2;
    auto yoffset = height / 2;
    for (const auto& p : hull2)
        vs[p.second.loc.y()][p.second.loc.x()] = p.second.col;
    for (const auto& s : vs)
        std::cout << s << '\n';

	auto part1 = hull.size();
	auto part2 = 0;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
