#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include <get_input.hpp>

struct Adj_tally {
    int open = 0, tree = 0, yard = 0;
};

class Landscape {
private:
    int ticks = 0;
    std::vector<std::string> land;
public:
    explicit Landscape(std::vector<std::string> input)
        : land{std::move(input)} { }

    void print_land() const;

    int count_resources() const;
    void run_small(int n) { while (ticks < n) tick(); }
    int run_large(int n);
private:
    void tick();
    char forrest_magic(int row, int col) const;
    Adj_tally count_adjacents(int r, int c) const;
};

void Landscape::print_land() const
{
    for (const auto& row : land) {
        for (const auto m : row)
            std::cout << m;
        std::cout << '\n';
    }
}

int Landscape::count_resources() const
{
    std::vector<int> trees;
    std::vector<int> yards;

    for (const auto& row : land) {
        trees.push_back(std::count_if(std::begin(row), std::end(row),
                    [](const auto ch) { return ch == '|'; }));
        yards.push_back(std::count_if(std::begin(row), std::end(row),
                    [](const auto ch) { return ch == '#'; }));
    }

    int trees_total = std::accumulate(std::begin(trees), std::end(trees), 0);
    int yards_total = std::accumulate(std::begin(yards), std::end(yards), 0);
    
    return trees_total * yards_total;
}

int Landscape::run_large(int n)
{
    int sample = 1000;
    int m = n - sample;

    std::vector<int> vi;
    vi.reserve(sample);

    while (ticks < sample) {
        tick();
        vi.push_back(count_resources());
    }

    int last = vi.back();
    auto it = std::find(std::rbegin(vi) + 1, std::rend(vi), last);
    auto dist = std::distance(std::rbegin(vi), it);
    auto index = m % dist;
    return *(std::rbegin(vi) + index);
}

void Landscape::tick()
{
    std::vector<std::string> working = land;

    for (size_t i = 0; i < land.size(); ++i) {
        for (size_t j = 0; j < land[i].size(); ++j) {
            working[i][j] = forrest_magic(i, j);
        }
    }

    ++ticks;
    land = std::move(working);
}

char Landscape::forrest_magic(int r, int c) const
{
    char curr = land[r][c], next = '\0';
    auto tally = count_adjacents(r, c);

    switch (curr) {
    case '.':               // open land
        next = tally.tree >= 3 ? '|' : '.';
        break;
    case '|':               // tree land
        next = tally.yard >= 3 ? '#' : '|';
        break;
    case '#':               // lumberyard
        next = tally.tree >= 1 && tally.yard >= 1 ? '#' : '.';
        break;
    default:
        break;
    }

    return next;
}

Adj_tally Landscape::count_adjacents(int r, int c) const
{
    Adj_tally tally;
    std::vector<char> vadj;

    for (size_t i = r == 0 ? 0 : r-1;
         i < land.size() && i <= size_t(r+1); ++i)
        for (size_t j = c == 0 ? 0 : c-1;
             j < land[i].size() && j <= size_t(c+1); ++j)
            switch (land[i][j]) {
            case '.':   ++tally.open;   break;
            case '|':   ++tally.tree;   break;
            case '#':   ++tally.yard;   break;
            default:                    break;
            }

    switch (land[r][c]) {
    case '.':   --tally.open;   break;
    case '|':   --tally.tree;   break;
    case '#':   --tally.yard;   break;
    default:                    break;
    }

    return tally;
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 18 - Settlers of The North Pole\n";

    auto input = utils::get_input_lines(argc, argv, "18");
    auto land = Landscape{input};
    
    land.run_small(10);
    auto part1 = land.count_resources();
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = land.run_large(1'000'000'000);
    std::cout << "Part 2: " << part2 << '\n';
}
