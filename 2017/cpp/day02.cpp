/*
 * Advent of Code 2017 
 *
 * Day 2 - Corruption Checksum
 * 
 * Part 1: Find the checksum of the spreadsheet data set. For each row find the
 * difference between the largest and smallest values. The sum of the
 * differences of each row is the checksum.
 *
 * Part 2: Find the sum of the results of the only even divisions in each row.
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// get input
std::vector<std::vector<int>> get_input(const std::string& fname)
{
    std::ifstream ifs {fname};
    if (!ifs) throw std::runtime_error("Could not read from file " + fname);

    std::vector<std::vector<int>> vv;

    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss {line};

        std::vector<int> v;
        for (int n; iss >> n; )
            v.push_back(n);

        vv.push_back(v);
    }

    return vv;
}

// Part 1
int get_diff(const std::vector<int>& v)
{
    int min = 0;
    int max = 0;
    for (auto& a : v)
        if (min == 0 && max == 0) {
            min = a;
            max = a;
        }
        else if (a < min)
            min = a;
        else if (max < a)
            max = a;

    return max - min;
}

int get_checksum(const std::vector<std::vector<int>>& vv)
{
    std::vector<int> row_diffs(vv.size());

    for (auto& v : vv) {
        row_diffs.push_back(get_diff(v));
    }

    int sum = 0;
    for (auto& diff : row_diffs)
        sum += diff;

    return sum;
}

// Part 2
int get_even_div(const std::vector<int>& v)
{
    for (auto a : v) {
        for (auto b : v)
            if (a != b && a % b == 0)
                return = a / b;
    }

    return 0;       // no even divisions
}

int get_even_div_sum(const std::vector<std::vector<int>>& vv)
{
    std::vector<int> even_divs(vv.size());

    for (auto& v : vv)
        even_divs.push_back(get_even_div(v));

    int sum = 0;
    for (auto& d : even_divs)
        sum += d;

    return sum;
}

int main(int argc, char *argv[])
try {
    std::string file = argc > 1 ? argv[1] : "";

    std::vector<std::vector<int>> input = get_input("./day02" + file + ".txt");

    int part_1 = get_checksum(input);
    int part_2 = get_even_div_sum(input);

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
