#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> parse_input()
{
    std::vector<int> vi;
    for (std::string s; std::cin >> s; ) {
        std::transform(std::begin(s), std::end(s), std::begin(s),
                [](const auto& ch) {
                    return (ch == 'F' || ch == 'L') ? '0' : '1';
                });
        vi.push_back(std::stoi(s.substr(0, 7), nullptr, 2) * 8 +
                std::stoi(s.substr(7), nullptr, 2));
    }
    return vi;
}

int main()
{
    std::vector<int> vi = parse_input();
    std::sort(std::begin(vi), std::end(vi));

    auto part1 = vi.back();
    auto part2 = 1 + *std::adjacent_find(std::begin(vi), std::end(vi),
            [](const auto& a, const auto& b) { return b != a + 1; });
    
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}
