#include <iostream>
#include <vector>
#include <algorithm>

#include <get_input.hpp>

constexpr int grid_size = 300;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Fuel_square {
    int x, y, size, total;
};

std::ostream& operator<<(std::ostream& os, const Fuel_square& fs)
{
    return os << fs.x << ',' << fs.y << ',' << fs.size;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Grid {
public:
    Grid(int s_no) : serial{s_no} { init_grid(); set_power_levels(); }

    Fuel_square get_largest_3x3() const;
    Fuel_square get_largest_possible() const;

private:
    void init_grid();
    void set_power_levels();

    int shift_x(int x) const { return x + 1; }
    int shift_y(int y) const { return y + 1; }
    int get_rack_id(int x) const { return shift_x(x) + 10; }    // MAGIC
    int get_power_lvl(int x, int y) const;
    Fuel_square sum_3x3(int x, int y) const;
    Fuel_square sum_kxk(int x, int y, int k) const;

    int serial;
    std::vector<std::vector<int>> grid;
};

Fuel_square Grid::get_largest_3x3() const
{
    std::vector<Fuel_square> vf;

    for (size_t i = 0; i < grid.size() - 3; ++i)
        for (size_t j = 0; j < grid[i].size() - 3; ++j) 
            vf.push_back(sum_3x3(j, i));

    return *std::max_element(std::begin(vf), std::end(vf),
            [](const auto& a, const auto& b) { return a.total < b.total; });
}

Fuel_square Grid::get_largest_possible() const
{
    std::vector<Fuel_square> vf;

    for (size_t k = grid_size - 1; k > 0; --k)
        for (size_t i = 0; i < grid.size() - k; ++i)
            for (size_t j = 0; j < grid[i].size() - k; ++j)
                vf.push_back(sum_kxk(j, i, k));

    return *std::max_element(std::begin(vf), std::end(vf),
            [](const auto& a, const auto& b) { return a.total < b.total; });
}

void Grid::init_grid()
{
    grid.resize(grid_size);
    for (auto& vi : grid)
        vi.resize(grid_size);
}

void Grid::set_power_levels()
{
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            grid[i][j] = get_power_lvl(j, i);
        }
    }
}

int Grid::get_power_lvl(int x, int y) const
{
    int rack_id = get_rack_id(x);
    int power_lvl = rack_id * shift_y(y);
    power_lvl += serial;
    power_lvl *= rack_id;
    power_lvl = power_lvl / 100 % 10;
    power_lvl -= 5;
    return power_lvl;
}

Fuel_square Grid::sum_3x3(int x, int y) const
{
    int total = 0;
    for (int i = y; i < y + 3; ++i)
        for (int j = x; j < x + 3; ++j)
            total += grid[i][j];

    return Fuel_square{shift_x(x), shift_y(y), 3, total};
}

Fuel_square Grid::sum_kxk(int x, int y, int k) const
{
    int total = 0;
    for (int i = y; i < y + k; ++i)
        for (int j = x; j < x + k; ++j)
            total += grid[i][j];

    return Fuel_square{shift_x(x), shift_y(y), k, total};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 11 - Chronal Charge\n";

    auto input = utils::get_input_string(argc, argv, "11");
    auto cells = Grid{std::stoi(input)};

    auto part1 = cells.get_largest_3x3();
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = cells.get_largest_possible();
    std::cout << "Part 2: " << part2 << '\n';
}
