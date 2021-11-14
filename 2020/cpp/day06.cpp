#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Read_input.hpp"

using Declarations = std::vector<std::pair<std::string,int>>;

auto process_input(const std::vector<std::string>& input)
{
    Declarations d { std::make_pair("", 0) };
    for (const auto& s : input) {
        if (s.empty()) {
            d.push_back(std::make_pair("", 0));
        } else {
            d.back().first += s;
            d.back().second += 1;
        }
    }
    for (auto& g : d)
        std::sort(std::begin(g.first), std::end(g.first));
    return d;
}

auto do_all_the_work(const Declarations& d)
{
    std::size_t count1 = 0, count2 = 0;
    for (const auto& [ g, n ] : d) {
        // part 1 - count any 'yes'
        std::string buff;
        std::unique_copy(std::begin(g), std::end(g), std::back_inserter(buff));
        count1 += buff.size();

        // part 2 - count consensus 'yes'
        std::vector<std::size_t> char_count;
        for (const auto ch : buff) {
            char_count.push_back(std::count(std::begin(g), std::end(g), ch));
        }
        count2 += std::count(std::begin(char_count), std::end(char_count), n);
    }
    return std::make_pair(count1, count2);
}

int main()
{
    const auto input = read_input_lines();
    const Declarations decls = process_input(input);

    const auto [ part1, part2 ] = do_all_the_work(decls);
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}