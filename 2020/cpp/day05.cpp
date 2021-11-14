#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Read_input.hpp"

int get_mid(const std::string& s, int high, int low = 0)
{
	auto mid = low + (high - low) / 2;
	for (const auto ch : s) {
		if (ch == 'F' || ch == 'L') {
			high = mid - 1;
		} else if (ch == 'B' || ch == 'R') {
			low = mid + 1;
		}
		mid = low + (high - low) / 2;
	}
	return mid;
}

int main()
{
	const auto vs = read_input<std::string>();

	std::vector<int> vi;
	for (const auto& s : vs) {
		auto row = get_mid(s.substr(0, 7), 127);
		auto col = get_mid(s.substr(7), 7);
		vi.push_back(row * 8 + col);
	}
	auto part1 = *std::max_element(std::begin(vi), std::end(vi));
	std::sort(std::begin(vi), std::end(vi));

	auto part2 = 1 + *std::adjacent_find(std::begin(vi), std::end(vi),
			[](const auto& a, const auto& b) { return b != a + 1; });

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}