#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>

std::vector<int> fft(const std::vector<int>& vin)
{
    std::vector<int> vout;

    static const std::vector<int> base_pat { 0, 1, 0, -1 };

    for (auto i = 0; i < vin.size(); ++i) {
        // get pattern
        std::vector<int> this_pat;
        for (const auto val : base_pat)
            for (auto k = 0; k < i + 1; ++k)
                this_pat.push_back(val);

        auto it = std::next(std::begin(this_pat));      // skip first value
        int sum = 0;
        for (const auto j : vin) {
            sum += j * (*it++);
            if (it == std::end(this_pat))
                it = std::begin(this_pat);
        }
        vout.push_back(std::abs(sum) % 10);
    }

    return vout;
}

std::string run_phases(std::vector<int> vi, int n, size_t substr_start = 0)
{
    constexpr auto substr_len = 8;

    for (auto i = 0; i < n; ++i)
        vi = fft(vi);

    std::string strout;
    std::transform(std::begin(vi), std::end(vi), std::back_inserter(strout),
            [](const auto& a) { return static_cast<char>(a) + '0'; });

    return strout.substr(substr_start, substr_len);
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 16\n"
              << "Flawed Frequency Transmission\n";

    std::vector<int> vin;
    for (char ch; std::cin >> ch; )
        vin.push_back(static_cast<int>(ch - '0'));

	std::cout << "Input size: " << vin.size() << '\n'
              << "  * first: " << vin.front() << '\n'
              << "  * last : " << vin.back() << '\n';

    std::vector<int> vbase { 0, 1, 0, -1 };

	auto part1 = run_phases(vin, 100);
	std::cout << "Part 1: " << part1 << '\n';

    std::vector<int> vin2;
    for (auto i = 0; i < 10000; ++i)
        std::copy(std::begin(vin), std::end(vin),
                std::back_inserter(vin2));

    std::string mos;
    std::transform(std::begin(vin), std::next(std::begin(vin), 7),
            std::back_inserter(mos),
            [](const auto& a) { return static_cast<char>(a) + '0'; });
    std::cout << "MOS: " << mos << '\n';

	auto part2 = run_phases(vin2, 100, std::stoi(mos));
	std::cout << "Part 2: " << part2 << '\n';
}
