#include <iostream>
#include <vector>
#include <algorithm>
#include "Read_input.hpp"

int find_summed_pair_prod(const std::vector<int>& vi, int target)
{
    for (auto it1 = std::begin(vi); it1 != std::end(vi); ++it1) {
        for (auto it2 = std::next(it1); it2 != std::end(vi); ++it2) {
            auto sum = *it1 + *it2;
            if (sum > target) {
                break;
            } else if (sum == target) {
                return *it1 * *it2;
            }
        }
    }
    return -1;
}

int64_t find_summed_trio_prod(const std::vector<int>& vi, int target)
{
    for (auto it1 = std::begin(vi); it1 != std::end(vi); ++it1) {
        for (auto it2 = std::next(it1); it2 != std::end(vi); ++it2) {
            for (auto it3 = std::next(it2); it3 != std::end(vi); ++it3) {
                auto sum = *it1 + *it2 + *it3;
                if (sum > target) {
                    break;
                } else if (sum == target) {
                    return *it1 * *it2 * *it3;
                }
            }
        }
    }
    return -1;
}

int main()
{
    const auto vi = read_input_and_sort<int>();

    auto part1 = find_summed_pair_prod(vi, 2020);
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = find_summed_trio_prod(vi, 2020);
    std::cout << "Part 2: " << part2 << '\n';
}