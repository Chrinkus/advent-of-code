#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <thread>
#include <array>

using Hill = std::vector<std::string>;
using Slope = std::pair<int,int>;
using Slopes = std::array<Slope,5>;

void count_trees(const Slope& slope, const Hill& hill, int& out)
{
    const auto [ right, down ] = slope;
    std::size_t x = 0, y = 0;
    while (y < hill.size()) {
        if (hill[y][x] == '#')
            ++out;
        x = (x + right) % hill.front().size();
        y += down;
    }
}

int64_t get_tree_sum_prod(const Slopes& slopes, const Hill& hill)
{
    std::vector<int> res (slopes.size(), 0);

    std::vector<std::thread> vt;
    for (std::size_t i = 0; i != slopes.size(); ++i) {
        vt.push_back(std::thread{count_trees, std::cref(slopes[i]),
                std::cref(hill), std::ref(res[i])});
    }
    for (auto& t : vt)
        t.join();
    return std::accumulate(std::begin(res), std::end(res), 1,
            std::multiplies<int>());
}

int main()
{
    constexpr Slopes slopes { { {1,1}, {3,1}, {5,1}, {7,1}, {1,2} } };

    Hill hill;
    for (std::string line; std::getline(std::cin, line); )
        hill.push_back(line);

    const auto part2 = get_tree_sum_prod(slopes, hill);
    std::cout << "Part 2: " << part2 << '\n';
}
