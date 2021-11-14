#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Read_input.hpp"       // simple, handy reader
#include "Timer.hpp"            // for timing solution
constexpr int WIDTH = 6;        // for output alignment

int main()
try {
    Timer t {};         // start of timing

    const auto input = read_input_lines();
    std::cout << "Size of input: " << input.size() << '\n'
              << "Front: " << input.front() << '\n'
              << "Back: " << input.back() << '\n';

    const auto part1 = 0;
    const auto part2 = 0;
    std::cout << std::setw(WIDTH) << "Part 1: " << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2: " << part2 << '\n';

    t.end(WIDTH);       // end of timing, print report
}
catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}