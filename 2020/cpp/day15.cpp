#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Timer.hpp"            // for timing solution
constexpr int WIDTH = 6;        // for output alignment

using Index = std::size_t;
constexpr Index TARGET_1 = 2020;
constexpr Index TARGET_2 = 30000000;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Custom read
auto read_starting_numbers()
{
    std::vector<Index> vi;
    for (Index n; std::cin >> n; ) {
        vi.push_back(n);
        std::cin.get();
    }
    return vi;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 1 and 2 use the same solution. Version 1 is brute-force-y.
auto play_until_n_v1(std::vector<Index> vi, const Index n)
{
    for (auto x = vi.back(); vi.size() < n; x = vi.back()) {
        auto rit = std::find(std::next(std::rbegin(vi)), std::rend(vi), x);
        if (rit != std::rend(vi)) {
            auto d = std::distance(std::begin(vi), rit.base());
            vi.push_back(vi.size() - d);
        } else {
            vi.push_back(0);
        }
    }
    for (const auto& i : vi)
        std::cout << i << '\n';
    return vi.back();
}
// Version 2 uses a hash table for faster finding.
auto play_until_n_v2(const std::vector<Index>& vi, const Index n)
{
    std::unordered_map<Index,Index> spoken;

    Index i = 1;                            // need to tag first as '1'
    for ( ; i < std::size(vi); ++i) {
        spoken[vi[i-1]] = i;
    }
    Index last = vi.back();                 // speak last, i is 3
    for ( ; i <= n; ++i) {
        if (i == n) break;
        if (spoken.find(last) != std::end(spoken)) {
            auto d = i - spoken[last];
            spoken[last] = i;
            last = d;
        } else {
            spoken[last] = i;
            last = 0;
        }
    }
    return last;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    Timer t {};         // start of timing (runs in ~4.8s!)

    const auto input = read_starting_numbers();
    const auto part1 = play_until_n_v2(input, TARGET_1);
    const auto part2 = play_until_n_v2(input, TARGET_2);

    std::cout << "Advent of Code 2020 - Day 15\n";
    std::cout << std::setw(WIDTH) << "Part 1:\t" << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2:\t" << part2 << '\n';

    t.end(WIDTH);       // end of timing, print report
}
