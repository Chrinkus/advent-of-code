#include <iostream>
#include <vector>
#include <numeric>

inline int calc_fuel(const int mass)
{
    return mass / 3 - 2;
}

int calc_fuel_fuel(const int mass)
{
    int fuel = calc_fuel(mass);

    return fuel > 0 ? fuel + calc_fuel_fuel(fuel) : 0;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 1\n";
    std::cout << "The Tyranny of the Rocket Equation\n";

	std::vector<int> vi;
	for (int val; std::cin >> val; )
		vi.push_back(val);

	std::cout << "Input size: " << vi.size() << '\n'
              << "  * first: " << vi.front() << '\n'
              << "  * last : " << vi.back() << '\n';

	int total_fuel = std::accumulate(std::begin(vi), std::end(vi), 0,
            [](const auto sum, const auto mass) {
                return sum + calc_fuel(mass);
            });

	int fuel_fuel = std::accumulate(std::begin(vi), std::end(vi), 0,
            [](const auto sum, const auto mass) {
                return sum + calc_fuel_fuel(mass);
            });

	std::cout << "Part 1: " << total_fuel << '\n';
	std::cout << "Part 2: " << fuel_fuel << '\n';
}
