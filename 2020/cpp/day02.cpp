#include <iostream>
#include <string>
#include <algorithm>

std::pair<int,int> count_valid_passwords()
{
    int lower, upper, count1 = 0, count2 = 0;
    char ch, xx;
    std::string password;
    while (std::cin >> lower >> xx >> upper >> ch >> xx >> password) {
        auto n = std::count(std::begin(password), std::end(password), ch);
        if (lower <= n && n <= upper)
            ++count1;
        
        --lower;
        --upper;
        if ((password[lower] == ch && password[upper] != ch)
                || (password[upper] == ch && password[lower] != ch))
            ++count2;
    }

    return std::make_pair(count1, count2);
}

int main()
{
    auto [ part1, part2 ] = count_valid_passwords();

    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}