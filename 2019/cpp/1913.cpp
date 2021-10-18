#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "Intcode_vm.hpp"
#include "Point.hpp"

constexpr int EMPTY = 0;
constexpr int WALL = 1;
constexpr int BLOCK = 2;
constexpr int PADDLE = 3;
constexpr int BALL = 4;

char getsym(const int id)
{
    switch (id) {
        case EMPTY:     return ' ';
        case WALL:      return '|';
        case BLOCK:     return '#';
        case PADDLE:    return '=';
        case BALL:      return '@';
        default:        return '?';
    }
}

struct Output {
    Point p;
    int id;
};

std::ostream& operator<<(std::ostream& os, const Output& o)
{
    return os << o.p << '\t' << getsym(o.id);
}

int main(int argc, char** argv)
{
    std::cout << "Advent of Code 2019 - Day 13\n"
              << "Care Package\n";

    std::ifstream ifs {argv[argc-1]};
    std::string input;
    ifs >> input;

    Intcode_vm vm {input};
    std::stringstream io;

    vm.run(std::cin, io);

    std::vector<Output> vo;
    for (int x, y, id; io >> x >> y >> id; )
        vo.emplace_back(Output{ Point{ x, y }, id });

	auto part1 = count_if(std::begin(vo), std::end(vo),
            [](const auto& o) {
                return o.id == BLOCK;
            });
	std::cout << "Part 1: " << part1 << '\n';

    auto max_x = std::max_element(std::begin(vo), std::end(vo),
            [](const auto& a, const auto& b) {
                return a.p.x() < b.p.x();
            })->p.x();
    auto max_y = std::max_element(std::begin(vo), std::end(vo),
            [](const auto& a, const auto& b) {
                return a.p.y() < b.p.y();
            })->p.y();

    std::vector<std::string> vs (max_y + 1);
    for (auto& s : vs)
        s.resize(max_x + 1, ' ');

    const Point tally { -1, 0 };
    Intcode_vm vm2 {input};
    vm2.zero(2);
    std::stringstream iss;
    std::stringstream oss;
    int64_t score = 0;
    while (!vm2.halted()) {
        // run machine, get new positions, should kick out at input
        vm2.run(iss, oss);
        std::vector<Output> vo2;
        for (int x, y, id; oss >> x >> y >> id; )
            vo2.emplace_back(Output{ Point{ x, y }, id });

        // extract data to field
        Point paddle;
        Point ball;
        for (const auto& o : vo2) {
            if (o.p == tally)
                score = o.id;
            else {
                if (o.id == PADDLE)
                    paddle = o.p;
                else if (o.id == BALL)
                    ball = o.p;
                vs[o.p.y()][o.p.x()] = getsym(o.id);
            }
        }

        /*
        // print field
        for (const auto& s : vs)
            std::cout << s << '\n';
        std::cout << "SCORE: " << score << '\n';
        */

        int inp = 0;
        /*
        // get player input
        std::cout << "Input: ";
        char ch;
        std::cin >> ch;
        switch (ch) {
            case 'a':   inp = -1;   break;
            case 's':   inp = 0;    break;
            case 'd':   inp = 1;    break;
        }
        */
        if (paddle.x() < ball.x())
            inp = 1;
        else if (paddle.x() > ball.x())
            inp = -1;
        else inp = 0;

        iss << inp << '\n';
        oss.clear();
    }

    auto part2 = score;
	std::cout << "Part 2: " << part2 << '\n';
}
