#include <iostream>
#include <vector>
#include <numeric>
#include <set>

#include <get_input.hpp>

int find_duplicate(const std::vector<int>& vi)
{
    std::set<int> si;

    int freq = 0;
    si.insert(freq);

    while (true)
        for (const auto val : vi) {
            freq += val;
            auto it = si.find(freq);
            if (it != si.end())
                return *it;
            si.insert(freq);
        }
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 1 - Chronal Calibration\n";
    auto vi = utils::get_input_values<int>(argc, argv, "01");

    auto part1 = std::accumulate(std::begin(vi), std::end(vi), 0);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = find_duplicate(vi);
    std::cout << "Part 2: " << part2 << '\n';

}
