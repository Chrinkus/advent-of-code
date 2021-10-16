#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>

#include <get_input.hpp>

class No_spring{};      // errors for throwing
class Bad_input{};
class Bad_fall{};
class Bad_flow{};

struct Point {
    int x, y;
};

struct Vein {
    Point start;
    Point end;
    Vein(int x1, int y1, int x2, int y2)
        : start{x1, y1}, end{x2, y2} { }

    int get_min_x() const { return std::min(start.x, end.x); }
    int get_min_y() const { return std::min(start.y, end.y); }
    int get_max_x() const { return std::max(start.x, end.x); }
    int get_max_y() const { return std::max(start.y, end.y); }
};

auto parse_input(const std::vector<std::string>& input)
{
    std::vector<Vein> vv;

    for (const auto& s : input) {
        int x1, y1, x2, y2;
        std::istringstream iss { s };
        char ch;
        iss >> ch;
        if (ch == 'x') {
            iss >> ch >> x1 >> ch;
            x2 = x1;
            iss >> ch >> ch >> y1 >> ch >> ch >> y2;
        } else if (ch == 'y') {
            iss >> ch >> y1 >> ch;
            y2 = y1;
            iss >> ch >> ch >> x1 >> ch >> ch >> x2;
        } else {
            std::cerr << "Bad input read: " << ch << '\n';
            throw Bad_input{};
        }
        vv.emplace_back(Vein{x1, y1, x2, y2});
    }
    return vv;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Ground_scan {
private:
    int min_x, min_y, max_x, max_y; 
    std::vector<std::string> scan;
public:
    explicit Ground_scan(const std::vector<Vein>& vd);

    void water_falls_like_rain_through_the_earth();

    int tally_watered() const;
    int tally_reserves() const;
    void print_scan(std::ostream& os = std::cout) const;
private:
    void fall(int level, int x);
    bool flow(int level, int x, int inc);
    void fill(int level, int x);

    int shift_x(int x) const { return x - min_x; }
    int shift_y(int y) const { return y - min_y; }
};

Ground_scan::Ground_scan(const std::vector<Vein>& vd)
{
    min_x = std::min_element(std::begin(vd), std::end(vd),
            [](const auto& a, const auto& b) {
                return a.get_min_x() < b.get_min_x();
            })->get_min_x();
    min_y = std::min_element(std::begin(vd), std::end(vd),
            [](const auto& a, const auto& b) {
                return a.get_min_y() < b.get_min_y();
            })->get_min_y();
    max_x = std::max_element(std::begin(vd), std::end(vd),
            [](const auto& a, const auto& b) {
                return a.get_max_x() < b.get_max_x();
            })->get_max_x();
    max_y = std::max_element(std::begin(vd), std::end(vd),
            [](const auto& a, const auto& b) {
                return a.get_max_y() < b.get_max_y();
            })->get_max_y();

    --min_y;            // make row for spring
    --min_x;            // widen for outside flow
    ++max_x;

    if (500 < min_x || max_x < 500)
        throw No_spring{};

    scan.resize(max_y + 1 - min_y);
    for (auto& row : scan) {
        row.resize(max_x + 1 - min_x);
        std::fill(std::begin(row), std::end(row), '.');     // fill w/sand
    }

    scan[0][shift_x(500)] = '+';                           // place spring

    for (const auto& v : vd) {
        for (int y = v.start.y; y <= v.end.y; ++y)
            for (int x = v.start.x; x <= v.end.x; ++x)
                scan[shift_y(y)][shift_x(x)] = '#';
    }
}

void Ground_scan::print_scan(std::ostream& os) const
{
    for (const auto row : scan) {
        for (const auto c : row)
            os << c;
        os << '\n';
    }
}

void Ground_scan::water_falls_like_rain_through_the_earth()
{
    int level = 1;
    int x = shift_x(500);

    fall(level, x);
}

int Ground_scan::tally_watered() const
{
    std::vector<int> vwatered;
    for (const auto& row : scan) {
        vwatered.push_back(
                std::count_if(std::begin(row), std::end(row),
                    [](const auto ch) { return ch == '|' || ch == '~'; })
                );
    }

    return std::accumulate(std::begin(vwatered), std::end(vwatered), 0);
}

int Ground_scan::tally_reserves() const
{
    std::vector<int> vwatered;
    for (const auto& row : scan) {
        vwatered.push_back(
                std::count_if(std::begin(row), std::end(row),
                    [](const auto ch) { return ch == '~'; })
                );
    }

    return std::accumulate(std::begin(vwatered), std::end(vwatered), 0);
}

void Ground_scan::fall(int level, int x)
{
    if (level >= static_cast<int>(scan.size()))
        return;

    bool left = false, right = false;

    switch (scan[level][x]) {
    case '.':                                   // sand
        scan[level][x] = '|';                     
        return fall(level + 1, x);
    case '~':                                   // fall into filled resevoir
    case '#':                                   // hit clay
        --level;                                // backup
        // fallthrough
    case '|':   // have backed up
        left = flow(level, x - 1, -1);
        right = flow(level, x + 1, 1);
        if (left && right)
            return fill(level, x);
        break;
    default:
        std::cerr << "Bad fall: " << level << ' ' << x << '\n';
        throw Bad_fall{};
    }
}

bool Ground_scan::flow(int level, int x, int inc)
{
    switch (scan[level][x]) {
    case '|':
    case '.':
        scan[level][x] = '|';
        switch (scan[level+1][x]) {
        case '.':
            fall(level + 1, x);
            return false;
        case '|':
            return false;
        case '~':
        case '#':
            return flow(level, x + inc, inc);
        }
    case '~':
        return flow(level - 1, x + inc, inc);
    case '#':
        return true;
    default:
        std::cerr << "Bad flow: " << scan[level][x]
                  << " level: " << level << " x: " << x << '\n';
        throw Bad_flow{};
    }
}

void Ground_scan::fill(int level, int x)
{
    int xl = x - 1, xr = x;
    while (scan[level][xl] == '|')
        scan[level][xl--] = '~';
    while (scan[level][xr] == '|')
        scan[level][xr++] = '~';
    return fall(--level, x);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 17 - Resevoir Research\n";

    auto input = utils::get_input_lines(argc, argv, "17");

    auto vein_data = parse_input(input);
    Ground_scan gs {vein_data};
    gs.water_falls_like_rain_through_the_earth();

    auto part1 = gs.tally_watered();
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = gs.tally_reserves();
    std::cout << "Part 2: " << part2 << '\n';
}
