#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>

#include <get_input.hpp>

class Coords {
public:
    Coords(char id, int x, int y) : _id{id}, _x{x}, _y{y} { }
    char id() const { return _id; }
    int  x()  const { return _x; }
    int  y()  const { return _y; }
    bool is_infinite() const { return infinite; }
    void is_infinite(bool b) { infinite = b; }
    void inc_area() { ++area; }
    int get_area() const { return area; }
private:
    char _id = '.';
    int _x = 0;
    int _y = 0;
    bool infinite = false;
    int area = 0;
};

std::ostream& operator<<(std::ostream& os, const Coords& c)
{
    return os << c.id() << ' ' << c.is_infinite() << ' ' << c.get_area();
}

auto parse_coords(const std::vector<std::string>& vs)
{
    std::vector<Coords> vc;
    char id = 'A';
    for (const auto& s : vs) {
        int x, y;
        char c;
        std::istringstream iss {s};
        iss >> x >> c >> y;
        vc.emplace_back(Coords{id, x, y});
        ++id;
    }

    return vc;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int man_dist(const Coords& c, int x, int y)
{
    return std::abs(c.x() - x) + std::abs(c.y() - y);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Grid {
public:
    Grid(const std::vector<std::string>& vs);

    void print_report() const;
    int shift_x(int x) const { return x + min_x; }
    int shift_y(int y) const { return y + min_y; }
    int max_finite_area() const;
    void paint_safe_region(int area);
    int get_safe_region_size() const;
private:
    char nearest_coords(int x, int y) const;
    void check_infinite_coords();
    void count_area();

    int min_x = -1;
    int min_y = -1;
    int max_x = -1;
    int max_y = -1;
    std::vector<Coords> coords;
    std::vector<std::vector<char>> grid;
};

Grid::Grid(const std::vector<std::string>& vs)
    : coords{parse_coords(vs)}
{
    min_x = std::max_element(std::begin(coords), std::end(coords),
            [](const Coords& a, const Coords& b) {
                return a.x() > b.x();
            })->x();
    min_y = std::max_element(std::begin(coords), std::end(coords),
            [](const Coords& a, const Coords& b) {
                return a.y() > b.y();
            })->y();
    max_x = std::max_element(std::begin(coords), std::end(coords),
            [](const Coords& a, const Coords& b) {
                return a.x() < b.x();
            })->x();
    max_y = std::max_element(std::begin(coords), std::end(coords),
            [](const Coords& a, const Coords& b) {
                return a.y() < b.y();
            })->y();

    // fill grid with default ids
    grid.resize(max_y + 1 - min_y);
    std::for_each(std::begin(grid), std::end(grid),
            [this](auto& vec) {
                vec.resize(this->max_x + 1 - this->min_x);
                std::fill(std::begin(vec), std::end(vec), '.');
            });

    // set grid tokens
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            grid[i][j] = nearest_coords(shift_x(j), shift_y(i));
        }
    }

    check_infinite_coords();
    count_area();
}

void Grid::print_report() const
{
    std::cout << "Max X: " << max_x << " Max Y: " << max_y << '\n';
    for (const auto& row : grid) {
        for (const auto ele : row)
            std::cout << ele;
        std::cout << '\n';
    }
    for (const auto& c : coords)
        std::cout << c << '\n';
}

char Grid::nearest_coords(int x, int y) const
{
    std::vector<std::pair<char,int>> vci;
    for (const auto& c : coords)
        vci.emplace_back(std::make_pair(c.id(), man_dist(c, x, y)));

    std::sort(std::begin(vci), std::end(vci),
            [](auto a, auto b) { return a.second < b.second; });

    if (vci[0].second == vci[1].second)
        return '.';
    else
        return vci[0].first;
}

void set_infinite(std::vector<Coords>& vc, char id)
{
    std::find_if(std::begin(vc), std::end(vc),
            [id](auto& c) { return c.id() == id; })->is_infinite(true);
}

void Grid::check_infinite_coords()
{
    char checked = '\0';
    for (const auto c : grid.front())
        if (checked != c) {
            set_infinite(coords, c);
            checked = c;
        }
    for (const auto c : grid.back())
        if (checked != c) {
            set_infinite(coords, c);
            checked = c;
        }

    char other_ch = '\0';
    std::for_each(++std::begin(grid), --std::end(grid),
            [this, &checked, &other_ch](auto& row) {
                if (checked != row.front())
                    set_infinite(this->coords, row.front());
                checked = row.front();

                if (other_ch != row.back())
                    set_infinite(this->coords, row.back());
                other_ch = row.back();
            });
}

void Grid::count_area()
{
    for (const auto& row : grid) {
        auto it = std::end(coords);
        for (const auto c : row) {
            if (it == std::end(coords) || it->id() != c)
                it = std::find_if(std::begin(coords), std::end(coords),
                        [c](auto& c_ref) { return c_ref.id() == c; });
            it->inc_area();
        }
    }
}

int Grid::max_finite_area() const
{
    return std::max_element(std::begin(coords), std::end(coords),
            [](auto& a, auto& b) {
                return !b.is_infinite() && a.get_area() < b.get_area();
            })->get_area();
}

void Grid::paint_safe_region(int area)
{
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            std::vector<int> dist (coords.size());
            std::transform(std::begin(coords),
                           std::end(coords),
                           std::begin(dist),
                           [this, i, j](auto& c) {
                               return man_dist(c, this->shift_x(j),
                                               this->shift_y(i));
                           });
            if (std::accumulate(std::begin(dist), std::end(dist), 0) < area)
                grid[i][j] = '#';
        }
    }
}

int Grid::get_safe_region_size() const
{
    int size = 0;
    for (const auto& row : grid)
        size += std::count_if(std::begin(row), std::end(row),
                [](const auto t) { return t == '#'; });

    return size;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 6 - Chronal Coordinates\n";

    auto input = utils::get_input_lines(argc, argv, "06");
    auto grid = Grid{input};

    auto part1 = grid.max_finite_area();
    std::cout << "Part 1: " << part1 << '\n';

    grid.paint_safe_region(10000);
    //grid.print_report();          //only run on test-input!!
    auto part2 = grid.get_safe_region_size();
    std::cout << "Part 2: " << part2 << '\n';
}
