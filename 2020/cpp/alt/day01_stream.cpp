#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using Int = std::size_t;

auto get_2020_sum(const std::vector<int>& vi, Int& i, Int& j)
{
    for ( ; i < j; ++i) {
        if (vi[i] + vi[j] == 2020)
            return vi[i] * vi[j];
    }
    i = 0;
    ++j;
    return 0;
}

auto get_2020_prod(const std::vector<int>& vi, Int& i, Int& j, Int& k)
{
    for ( ; i < j; ++i) {
        for ( ; j < k; ++j) {
            if (vi[i] + vi[j] + vi[k] == 2020)
                return vi[i] * vi[j] * vi[k];
        }
        // something here
    }
    i = 0;
    j = 1;
    ++k;
    return 0;
}

auto get_2020()
{
    auto sum = 0, prod = 0;
    Int i = 0, j = 1, k = 0, l = 1, m = 2;
    std::vector<int> vi;
    for (auto n = 0; std::cin >> n; ) {
        vi.push_back(n);
        if (sum == 0 && vi.size() > 1)
            sum = get_2020_sum(vi, i, j);
        if (prod == 0 && vi.size() > 2)
            prod = get_2020_prod(vi, k, l, m);
        if (sum != 0 && prod != 0)
            break;
    }
    return std::make_pair(sum, prod);
}

int main()
{
    const auto [ part1, part2 ] = get_2020();

    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}