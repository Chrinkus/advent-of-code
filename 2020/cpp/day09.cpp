#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Read_input.hpp"

using Index = std::size_t;      // I really dislike typing this
using Value = std::uint64_t;    // Also this..

Value find_weakness(const std::vector<Value>& vi, const Value preamble = 25)
{
    bool found_pair = false;
    for (Index i = preamble, j = 0, k = 1; i < vi.size(); ++i) {
        for (j = i - preamble; j < i - 1; ++j) {
            for (k = j + 1; k < i; ++k) {
                found_pair = vi[i] == vi[j] + vi[k];
                if (found_pair)
                    break;
            }
            if (found_pair)
                break;
        }
        if (!found_pair)
            return vi[i];
    }
    return 0;
}

Value break_encryption(const std::vector<Value>& vi, const Value target)
{
    for (Index gap = 1; gap < vi.size(); ++gap) {
        for (auto it = std::begin(vi), jt = std::next(it, gap);
                jt < std::end(vi); ++it, ++jt) {
                    if (std::accumulate(it, std::next(jt), Value{}) == target) {
                        auto min = *std::min_element(it, std::next(jt));
                        auto max = *std::max_element(it, std::next(jt));
                        return min + max;
                    }
                }
    }
    return 0;
}

int main()
{
    const auto input = read_input<Value>();

    const auto part1 = find_weakness(input, 25);
    const auto part2 = break_encryption(input, part1);
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}
