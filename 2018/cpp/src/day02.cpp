#include <iostream>
#include <vector>
#include <algorithm>

#include <get_input.hpp>

std::pair<bool,bool> check_id(std::string s)
{
    auto bools = std::make_pair(false, false);      // early initialization
                                                    // == return optimization?
    std::sort(std::begin(s), std::end(s));

    char prev = '\0';
    int count = 0;
    for (auto it = std::begin(s); it <= std::end(s); ++it) {    // it <= end
        if (it != std::end(s) && *it == prev) {     // protects against *end
            ++count;
        } else {
            if (count == 2)
                bools.first = true;
            else if (count == 3)
                bools.second = true;
            count = 1;
            prev = *it;
        }
    }
    return bools;
}

int get_checksum(const std::vector<std::string>& vs)
{
    auto checksum = std::make_pair(0, 0);

    std::for_each(std::begin(vs), std::end(vs),
            [&checksum](std::string s) {
                auto res = check_id(s);
                checksum.first += res.first;    // implicit bool to int: is
                checksum.second += res.second;  // this a bad thing?
            });

    return checksum.first * checksum.second;
}

std::string get_common_id(const std::string& s1, const std::string& s2)
{
    auto it = std::mismatch(std::begin(s1), std::end(s1),
                            std::begin(s2)).first;
    return std::string{std::begin(s1), it}.append(++it, std::end(s1));
}

std::string find_almost_match(const std::vector<std::string>& vs)
{
    for (const auto& s1 : vs)
        for (const auto& s2 : vs) {
            if (&s1 != &s2) {       // compare pointers instead of strings
                auto it_pair = std::mismatch(std::begin(s1), std::end(s1),
                                             std::begin(s2));
                auto it = std::mismatch(++it_pair.first, std::end(s1),
                                        ++it_pair.second).first;
                if (it == std::end(s1))
                    return get_common_id(s1, s2);
            }
        }
    return "Error: could not find near match\n";
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 2 - Inventory Management System\n";
    auto input = utils::get_input_lines(argc, argv, "02");

    auto part1 = get_checksum(input);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = find_almost_match(input);
    std::cout << "Part 2: " << part2 << '\n';
}
