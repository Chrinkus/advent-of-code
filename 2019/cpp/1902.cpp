#include <iostream>
#include <vector>

int run_intcode(std::vector<int> vi, int noun, int verb)
{
    vi[1] = noun;
    vi[2] = verb;
    for (size_t i = 0; i < vi.size(); i += 4) {
        int op = vi[i];
        int r1 = vi[i+1];
        int r2 = vi[i+2];
        int st = vi[i+3];
        switch (op) {
            case 1:     vi[st] = vi[r1] + vi[r2];   break;
            case 2:     vi[st] = vi[r1] * vi[r2];   break;
            case 99:    return vi[0];
        }
    }
    return -1;
}

int hit_target(std::vector<int> vi, int target)
{
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            if (run_intcode(vi, i, j) == target) {
                std::cout << "noun: " << i << " verb: " << j << '\n';
                return 100 * i + j;
            }
    return -1;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 2\n"
              << "1202 Program Alarm\n";

	std::vector<int> vi;
    char ch;
	for (int val; std::cin >> val; std::cin >> ch)
		vi.push_back(val);

    /*
	std::cout << "Input size: " << vi.size() << '\n'
              << "  * first: " << vi.front() << '\n'
              << "  * last : " << vi.back() << '\n';
    */

	int part1 = run_intcode(vi, 12, 2);

    int target = 19690720;
	int part2 = hit_target(vi, target);

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
