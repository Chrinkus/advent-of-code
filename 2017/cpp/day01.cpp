/*
 * Advent of Code 2017 
 *
 * Day 1 - Inverse Captcha
 * 
 * Part 1: Find the sum of all digits that match the next digit in a data set. Check
 * for circular match also.
 *
 * Part 2: Find the sum of all digits that match the digit exactly halfway
 * around the data set. Ok to assume even number of elements.
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

int char_to_int(char ch)
{
    int n = ch - '0'; 
    return n;
}

template<typename Func>
std::vector<int> get_file_input(const std::string& fname, Func fn)
{
    std::ifstream ifs {fname};
    if (!ifs)
        throw std::runtime_error("Could not open file " + fname);

    std::vector<int> v;
    for (char ch; ifs >> ch; )
        v.push_back(fn(ch));

    return v;
}

int consecutive_match_sum(const std::vector<int>& v)
{
    int sum = 0;
    int prev = v.back();

    for (auto& a : v) {
        if (prev == a)
            sum += prev;
        prev = a;
    }

    return sum;
}

int distant_match_sum(const std::vector<int>& v)
{
    int sum = 0;
    int half = v.size() / 2;

    for (int i = 0; i < half; ++i)
        if (v[i] == v[i + half])
            sum += v[i] * 2;

    return sum;
}

int main()
try {
    std::vector<int> values = get_file_input("./day01.txt",
            [](char ch) { return ch - '0'; });

    int part_1 = consecutive_match_sum(values);
    int part_2 = distant_match_sum(values);

    std::cout << "The solution to part 1 is: " << part_1 << '\n';
    std::cout << "The solution to part 2 is: " << part_2 << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
