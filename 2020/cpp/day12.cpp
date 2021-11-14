#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Timer.hpp"            // for timing solution
constexpr int WIDTH = 8;        // for output alignment

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
enum class Facing { east, north, west, south };

Facing operator++(Facing& f)
{
    f = (f==Facing::south) ? Facing::east : Facing(int(f)+1);
    return f;
}

Facing operator--(Facing& f)
{
    f = (f==Facing::east) ? Facing::south : Facing(int(f)-1);
    return f;
}

enum class Dir { left, right };
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class Ferry {
public:
    Ferry() = default;
    void process_nav_ins(const std::pair<char,int>& ins);
    void process_way_ins(const std::pair<char,int>& ins);
    int man_dist() const { return std::abs(x) + std::abs(y); }
private:
    // Part 1
    void strafe(const int dx, const int dy) { x += dx; y += dy; }
    void turn(const Dir dir, int deg);
    void advance(const int arg);

    Facing f = Facing::east;
    int x = 0;
    int y = 0;

    // Part 2
    void move_waypoint(const int dx, const int dy) { wpx += dx; wpy += dy; }
    void rotate_waypoint(const Dir dir, const int deg);
    void sail_to_waypoint(const int units);

    int wpx = 10;       // relative to ferry's position
    int wpy = -1;
};
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 1
void Ferry::process_nav_ins(const std::pair<char,int>& ins)
{
    const auto [ ch, arg ] = ins;
    switch (ch) {
    case 'N':   strafe(0, -arg);        break;
    case 'S':   strafe(0, arg);         break;
    case 'E':   strafe(arg, 0);         break;
    case 'W':   strafe(-arg, 0);        break;
    case 'L':   turn(Dir::left, arg);   break;
    case 'R':   turn(Dir::right, arg);  break;
    case 'F':   advance(arg);           break;
    }
}

void Ferry::turn(const Dir dir, int deg)
{
    if (dir == Dir::left)
        while (deg) { ++f; deg -= 90; }
    else
        while (deg) { --f; deg -= 90; }
}

void Ferry::advance(const int arg)
{
    switch (f) {
    case Facing::east:  x += arg;   break;
    case Facing::north: y -= arg;   break;
    case Facing::west:  x -= arg;   break;
    case Facing::south: y += arg;   break;
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 2
void Ferry::process_way_ins(const std::pair<char,int>& ins)
{
    const auto [ ch, arg ] = ins;
    switch (ch) {
    case 'N':   move_waypoint(0, -arg);             break;
    case 'S':   move_waypoint(0, arg);              break;
    case 'E':   move_waypoint(arg, 0);              break;
    case 'W':   move_waypoint(-arg, 0);             break;
    case 'L':   rotate_waypoint(Dir::left, arg);    break;
    case 'R':   rotate_waypoint(Dir::right, arg);   break;
    case 'F':   sail_to_waypoint(arg);              break;
    }
}

void Ferry::rotate_waypoint(const Dir dir, const int deg)
{
    auto tmp = 0;
    if (deg == 180) {
        wpx = -wpx; wpy = -wpy;
    } else if (dir == Dir::left) {
        switch (deg) {
        case 90:    tmp = wpx; wpx = wpy; wpy = -tmp;   break;
        case 270:   tmp = wpx; wpx = -wpy; wpy = tmp;   break;
        }
    } else {
        switch (deg) {
        case 90:    tmp = wpx; wpx = -wpy; wpy = tmp;   break;
        case 270:   tmp = wpx; wpx = wpy; wpy = -tmp;   break;
        }
    }
}

void Ferry::sail_to_waypoint(const int units)
{
    x += units * wpx;
    y += units * wpy;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
auto parse_nav()
    // read each line as a char instruction and int argument
{
    std::vector<std::pair<char,int>> vci;
    char ch;
    int arg;
    while (std::cin >> ch >> arg) {
        vci.push_back(std::make_pair(ch, arg));
    }
    return vci;
}

int main()
{
    Timer t {};         // start of timing

    const auto input = parse_nav();

    Ferry ferry_1 {};
    Ferry ferry_2 {};
    for (const auto& instruction : input) {
        ferry_1.process_nav_ins(instruction);
        ferry_2.process_way_ins(instruction);
    }
    const auto part1 = ferry_1.man_dist();
    const auto part2 = ferry_2.man_dist();
    std::cout << std::setw(WIDTH) << "Part 1" << ':' << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2" << ':' << part2 << '\n';
    
    t.end(WIDTH);       // end of timing, print report
}
