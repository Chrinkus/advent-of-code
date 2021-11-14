#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>               // for timing solution

#include "Read_input.hpp"       // simple lib for reading input from istreams

using Big_int = std::int64_t;

auto find_joltage_dist(const std::vector<int>& vi)
{
    auto count1 = 0, count3 = 0, prev = 0;
    std::for_each(std::begin(vi), std::end(vi), [&](const auto& n) {
        switch (n - prev) {
            case 1:     ++count1;               break;
            case 2:     /* don't count 2's */   break;
            case 3:     ++count3;               break;
        }
        prev = n;
    });
    ++count3;       // last jump is always a '3'

    return count1 * count3;
}

Big_int comb_magic(const int ones, const int jump = 3)
    // combinations seem to increase by a factor of 2
    // minus the amount by which ones exceed jump dist (3)
{
    return (1 << (ones - 1)) - (ones > jump ? ones - jump : 0);
}

auto map_distances(const std::vector<int>& vi)
{
    std::vector<int> distances;
    int prev = 0;
    for (const auto& i : vi) {
        distances.push_back(i - prev);
        prev = i;
    }
    distances.push_back(3); // last jump is always 3
    return distances;
}

auto count_combinations(const std::vector<int>& vi)
{
    std::vector<int> distances = map_distances(vi);

    Big_int ways = 1;       // "More than a trillion, Marty!"
    int ones = 0;           // consecutive 1's add possible combinations
    for (const auto& d : distances) {
        switch (d) {
        case 1:     ++ones;                     break;
        case 2:     /* there's never any 2's */ break;
        case 3:
            if (ones > 1) {
                ways *= comb_magic(ones);
            }
            ones = 0;       // reset ones on any 3
            break;
        }
    }
    return ways;
}

int main()
{
    // start of timing
    auto start = std::chrono::steady_clock::now();

    const auto input = read_input_and_sort<int>();

    const auto part1 = find_joltage_dist(input);
    const auto part2 = count_combinations(input);
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';

    // end of timing and report
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "Time: " << time.count() << '\n';
}
