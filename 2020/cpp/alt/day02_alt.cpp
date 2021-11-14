#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "Read_input.hpp"

auto count_valid_passwords(const std::vector<std::string>& input)
{
	int count1 = 0; int count2 = 0;
	for (const auto& line : input) {
		std::istringstream iss {line};
		int low, hi, flag = 0;
		char ch, xx;
		std::string pw;
		iss >> low >> xx >> hi >> ch >> xx >> pw;
		auto n = std::count(std::begin(pw), std::end(pw), ch);
		if (low <= n && n <= hi) ++count1;
		if (pw[--low] == ch) ++flag;
		if (pw[--hi]  == ch) ++flag;
		count2 += flag % 2;
	}
	return std::make_pair(count1, count2);
}

int main()
{
	const auto input = read_input_lines();

	const auto [ part1, part2 ] = count_valid_passwords(input);
	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}