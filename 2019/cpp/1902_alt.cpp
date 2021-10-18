#include <iostream>
#include <vector>
#include <string>

#include "Intcode_vm.hpp"

int brute_force_solve(const Intcode_vm& src, int target)
{
    for (auto i = 0; i < 100; ++i)
        for (auto j = 0; j < 100; ++j) {
            Intcode_vm vm {src};
            vm.config(i, j);
            vm.run();
            if (vm.zero() == target)
                return i * 100 + j;
        }
    return -1;
}

int double_binary_search(const Intcode_vm& src, int target)
{
    const int MAX = 100;
    int m = MAX / 2;
    int i = m;
    int j = m;
    while ((m /= 2)) {
        Intcode_vm vm {src};
        vm.config(i, 0);
        vm.run();
        int res = vm.size();
        std::cout << "m: " << m << " i: " << i << " res: " << res << '\n';
        if (res == target)
            break;
        else if (res < target)
            i += m;
        else if (res > target)
            i -= m;
    }
    return i;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 2 Alternate\n"
              << "1202 Program Alarm\n";

    std::string input;
    std::cin >> input;
    Intcode_vm vm1 {input};
    Intcode_vm vm2 {input};

	std::cout << "Input size: " << vm1.size() << '\n';

    vm1.config(12, 2);
    vm1.run();
	int part1 = vm1.zero();

    const int target = 19690720;
	int part2 = brute_force_solve(vm2, target);
	//int part2 = double_binary_search(vm2, target);

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
