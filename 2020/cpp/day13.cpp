#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Read_input.hpp"       // simple, handy reader
#include "Timer.hpp"            // for timing solution

using Index = std::size_t;
using Bigint = std::uint64_t;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
auto read_input()
{
    int timestamp;
    std::cin >> timestamp;

    std::vector<int> buses;
    for (int bus; std::cin; ) {
        char ch;
        std::cin >> bus;
        if (std::cin.eof()) {
            break;
        } else if (std::cin.fail()) {
            std::cin.clear();
            std::cin >> ch;
            if (ch != 'x') {
                std::cerr << "Error unknown char: " << ch << '\n';
            }
        }
        buses.push_back(bus);   // keep 0's
        std::cin >> ch;         // burn the comma
    }
    return std::make_pair(timestamp, buses);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
auto calc_next_arrivals(const int t, const std::vector<int>& sched)
{
    std::vector<std::pair<int,int>> arrivals;
    for (const auto& bus : sched) {
        if (bus == 0) continue;         // skip 0's
        auto mul = (t / bus) + 1;
        arrivals.push_back(std::make_pair(bus, bus * mul));
    }
    return arrivals;
}

auto get_next_id_and_minute(const int t,
        const std::vector<std::pair<int,int>>& arriv)
{
    std::vector<int> mins(arriv.size());
    std::transform(std::begin(arriv), std::end(arriv), std::begin(mins),
            [&t](const auto& a) { return a.second - t; });

    auto it = std::min_element(std::begin(mins), std::end(mins));
    Index i = std::distance(std::begin(mins), it);

    return arriv[i].first * *it;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
auto win_contest(const std::vector<int>& buses)
{
    std::vector<std::pair<int,int>> boffs;      // buses and offsets
    for (Index i = 0; i < buses.size(); ++i)
        if (buses[i] != 0)
            boffs.push_back(std::make_pair(i, buses[i]));
    
    auto it = std::begin(boffs);
    Bigint mins = (*it).second;
    Bigint stride = mins;
    for (it = std::next(it); it != std::end(boffs); ++it) {
        auto [off,val] = *it;
        while ((mins + off) % val != 0)
            mins += stride;
        stride *= val;
    }
    return mins;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    Timer t {};         // start of timing

    const auto [ timestamp, buses ] = read_input();
    const auto arriv = calc_next_arrivals(timestamp, buses);

    const auto part1 = get_next_id_and_minute(timestamp, arriv);
    const auto part2 = win_contest(buses);

    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';

    t.end(0);           // end of timing, print report
}