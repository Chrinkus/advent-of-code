#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Point {
public:
    Point() = default;
    Point(int x, int y) : xx{x}, yy{y} { }

    int x() const { return xx; }
    int y() const { return yy; }
    int manhatten_distance(const Point& p) const;

    void move(int dx, int dy) { xx += dx; yy += dy; }
private:
    int xx = 0;
    int yy = 0;
};

int Point::manhatten_distance(const Point& p) const
{
    return std::abs(xx - p.x()) + std::abs(yy - p.y());
}

bool operator<(const Point& a, const Point& b)
{
    return a.manhatten_distance(Point{}) < b.manhatten_distance(Point{});
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Square {
public:
    Square() = default;
    Square(const Point& p) : pt{p} { }

    std::string coords() const;
    Point location() const { return pt; }
    bool intersection() const { return wa > 0 && wb > 0; }
    int step_distance() const;

    void intersect(char id, int steps);
private:
    Point pt;
    int wa = 0;
    int wb = 0;
};

std::string Square::coords() const
{
    std::ostringstream oss;
    oss << pt.x() << ',' << pt.y();
    return oss.str();
}

void Square::intersect(char id, int steps)
{
    switch (id) {
        case 'a':
            if (wa == 0)
                wa = steps;
            break;
        case 'b':
            if (wb == 0)
                wb = steps;
            break;
        default:
            break;
    }
}

int Square::step_distance() const
{
    return intersection() ? wa + wb : -1;
}

bool operator<(const Square& a, const Square& b)
{
    return a.location() < b.location();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Move {
    Move(char d, int n) : dir{d}, num{n} { }
    char dir;
    int num;
};

std::ostream& operator<<(std::ostream& os, const Move& m)
{
    return os << m.dir << m.num;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Wire {
public:
    Wire() = delete;
    Wire(char idd) : id{idd}, loc{Point{0, 0}} { }

    void process_move(std::unordered_map<std::string,Square>& um,
            const Move& m);
private:
    char id;
    Point loc;
    int steps = 0;
};

void Wire::process_move(std::unordered_map<std::string,Square>& um,
        const Move& m)
{
    int dx, dy;
    switch (m.dir) {
        case 'U':   dx = 0; dy = 1;     break;
        case 'R':   dx = 1; dy = 0;     break;
        case 'D':   dx = 0; dy = -1;    break;
        case 'L':   dx = -1; dy = 0;    break;
        default:                        break;
    }

    for (auto i = 0; i < m.num; ++i) {
        loc.move(dx, dy);
        ++steps;
        Square sq {loc};
        std::string coords = sq.coords();

        auto it = um.find(coords);
        if (it == std::end(um))
            um[coords] = sq;
        um.at(coords).intersect(id, steps);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

void parse_moves(const std::string& move_list, std::vector<Move>& vm)
{
    std::istringstream iss {move_list};
    char ch;
    int num;
    while (iss >> ch >> num) {
        vm.emplace_back(Move{ch, num});
        iss >> ch;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main()
{
    std::cout << "Advent of Code 2019 - Day 3\n"
              << "Crossed Wires\n";

    std::string moves_1;
    std::cin >> moves_1;
	std::vector<Move> vm1;
    parse_moves(moves_1, vm1);

    std::string moves_2;
    std::cin >> moves_2;
	std::vector<Move> vm2;
    parse_moves(moves_2, vm2);

	std::cout << "Input size: " << vm1.size() << '\n'
              << "  * first: " << vm1.front() << '\n'
              << "  * last : " << vm1.back() << '\n';

	std::cout << "Input size: " << vm2.size() << '\n'
              << "  * first: " << vm2.front() << '\n'
              << "  * last : " << vm2.back() << '\n';

    Wire wa {'a'};
    Wire wb {'b'};
    std::unordered_map<std::string,Square> grid;
    for (const auto& m : vm1)
        wa.process_move(grid, m);

    for (const auto& m : vm2)
        wb.process_move(grid, m);

    std::cout << "Grid size: " << grid.size() << '\n';

    std::vector<Square> vs;
    for (const auto& sq : grid)
        if (sq.second.intersection())
            vs.push_back(sq.second);

    std::cout << "Intersected squares: " << vs.size() << '\n';
    std::sort(std::begin(vs), std::end(vs));

	int part1 = vs.front().location().manhatten_distance(Point{});
	int part2 = std::min_element(std::begin(vs), std::end(vs),
                [](const auto& a, const auto& b) {
                    return a.step_distance() < b.step_distance();
                })->step_distance();

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
