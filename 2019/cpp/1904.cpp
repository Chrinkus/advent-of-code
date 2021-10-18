#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

template<typename ForwardIt>
ForwardIt adjacent_find_n(ForwardIt first, ForwardIt last, std::size_t n)
    // adapted from cpprefernce.com's adjacent_find() implementation
{
    if (first == last)
        return last;
    std::size_t count = 1;
    ForwardIt next = std::next(first);
    for (; next != last; ++first, ++next) {
        if (*first == *next) {
            ++count;
        } else if (count == n) {
            return first;
        } else {
            count = 1;
        }
    }
    return count == n ? first : last;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 4\n"
              << "Secure Container\n";

    int min_passwd, max_passwd;
    std::cin >> min_passwd >> max_passwd;

    int count1 = 0;
    int count2 = 0;
    for (int i = min_passwd; i <= max_passwd; ++i) {
        std::stringstream ss;
        ss << i;
        std::string s = ss.str();

        if (!std::is_sorted(std::begin(s), std::end(s)))
            continue;

        if (std::adjacent_find(std::begin(s), std::end(s)) != std::end(s))
            ++count1;

        if (adjacent_find_n(std::begin(s), std::end(s), 2) != std::end(s))
            ++count2;
    }

	std::cout << "Part 1: " << count1 << '\n';
	std::cout << "Part 2: " << count2 << '\n';
}
