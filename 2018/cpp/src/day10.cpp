#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
#include <fstream>

#include <get_input.hpp>

struct Light {
    void advance() { x += dx; y += dy; }
    void rewind()  { x -= dx; y -= dy; }
    int x, y, dx, dy;
};

auto parse_light_input(const std::vector<std::string>& input)
{
    std::vector<Light> vlights;

    static const std::regex pat {
        R"(^position=<\s?(-?\d+),\s+(-?\d+).*<\s?(-?\d+),\s+(-?\d+)>$)"
    };

    for (const auto& line : input) {
        std::smatch matches;
        std::regex_match(line, matches, pat);
        if (!matches.empty()) {
            auto it = ++std::begin(matches);
            int x, y, dx, dy;
            x  = std::stoi(*it++);
            y  = std::stoi(*it++);
            dx = std::stoi(*it++);
            dy = std::stoi(*it++);
            vlights.emplace_back(Light{x, y, dx, dy});
        } else {
            std::cout << "Problem: No match! Check your foolish regex..\n";
        }
    }

    return vlights;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Min_max {
    explicit Min_max(const std::vector<Light>& vlights);

    int get_width()  const { return max_x - min_x; }
    int get_height() const { return max_y - min_y; }

    int min_x, min_y, max_x, max_y;
};

Min_max::Min_max(const std::vector<Light>& vlights)
{
    min_x = std::min_element(std::begin(vlights), std::end(vlights),
            [](const Light& a, const Light& b) { return a.x < b.x; })->x;
    min_y = std::min_element(std::begin(vlights), std::end(vlights),
            [](const Light& a, const Light& b) { return a.y < b.y; })->y;
    max_x = std::max_element(std::begin(vlights), std::end(vlights),
            [](const Light& a, const Light& b) { return a.x < b.x; })->x;
    max_y = std::max_element(std::begin(vlights), std::end(vlights),
            [](const Light& a, const Light& b) { return a.y < b.y; })->y;
}

bool operator<(const Min_max& a, const Min_max& b)
{
    return a.get_height() < b.get_height();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Grid {
public:
    Grid(const std::vector<Light>& vlights);

    void print_sky() const;
private:

    int shift_x(int x) const { return x - mm.min_x; }
    int shift_y(int y) const { return y - mm.min_y; }

    Min_max mm;
    std::vector<std::vector<char>> grid;
};

Grid::Grid(const std::vector<Light>& vlights)
    : mm{vlights}
{
    grid.resize(mm.max_y + 1 - mm.min_y);
    for (auto& vc : grid) {
        vc.resize(mm.max_x + 1 - mm.min_x);
        std::fill(std::begin(vc), std::end(vc), '.');
    }

    for (const auto& l : vlights) {
        grid[shift_y(l.y)][shift_x(l.x)] = '#';
    }
}

void Grid::print_sky() const
{
    for (const auto& vl : grid) {
        for (const auto l : vl)
            std::cout << l;
        std::cout << '\n';
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Star_field {
public:
    Star_field(const std::vector<std::string>& input)
        : vlights{parse_light_input(input)}, mm{vlights} { }

    const std::vector<Light>& get_vlights() const { return vlights; }

    int time_to_shortest();
private:
    void advance_all();
    void rewind_all();

    std::vector<Light> vlights;
    Min_max mm;
};

int Star_field::time_to_shortest()
{
    int old_height, time_s = 0;
    do {
        old_height = mm.get_height();
        advance_all();
        ++time_s;
    } while (old_height > mm.get_height());
    rewind_all();
    --time_s;
    
    return time_s;
}

void Star_field::advance_all()
{
    for (auto& l : vlights)
        l.advance();
    mm = Min_max{vlights};
}

void Star_field::rewind_all()
{
    for (auto& l : vlights)
        l.rewind();
    mm = Min_max{vlights};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 10 - The Stars Align\n";

    auto input = utils::get_input_lines(argc, argv, "10");
    Star_field sf {input};
    auto time_s = sf.time_to_shortest();
    Grid sky {sf.get_vlights()};
    sky.print_sky();
    std::cout << "Time till message: " << time_s << '\n';
}
