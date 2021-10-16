#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#include <get_input.hpp>
#include <task_threading.hpp>

bool are_opposite(char a, char b)
{
    if (isupper(a) && tolower(a) == b)
        return true;
    else if (islower(a) && toupper(a) == b)
        return true;
    else
        return false;
}

int react_polymer(std::string s)
{
    auto it = std::begin(s);
    auto sentry = std::end(s);

    while (it != sentry) {
        auto nit = it + 1;
        if (nit != sentry && are_opposite(it[0], nit[0])) {
            it = s.erase(it, nit + 1);
            sentry = std::end(s);

            if (it != std::begin(s))
                --it;
        } else {
            ++it;
        }
    }

    return s.size();
}

std::string remove_all_of_char(std::string s, char c)
{
    auto it = std::begin(s);
    auto sentry = std::end(s);
    while (it != sentry) {
        if (it[0] == c || it[0] == toupper(c)) {
            it = s.erase(it);
            sentry = std::end(s);
        } else {
            ++it;
        }
    }
    return s;
}

int reduce_react(std::string s, char c)
{
    s = remove_all_of_char(s, c);       // reduce
    return react_polymer(s);            // react
}

int get_shortest_length(const std::string& s)
{
    std::vector<char> alphabet;
    for (char c = 'a'; c <= 'z'; ++c)
        alphabet.push_back(c);

    std::vector<int> lengths (alphabet.size());
    utils::split_task(reduce_react, alphabet, lengths, s);

    auto it_min = std::min_element(std::begin(lengths), std::end(lengths));
    return *it_min;
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 5 - Alchemical Reduction\n";

    auto input = utils::get_input_string(argc, argv, "05");

    auto part1 = react_polymer(input);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = get_shortest_length(input);
    std::cout << "Part 2: " << part2 << '\n';
}
