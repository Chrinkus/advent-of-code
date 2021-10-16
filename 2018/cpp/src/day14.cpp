#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <get_input.hpp>

class Kitchen {
private:
    size_t e1 = 0;
    size_t e2 = 1;
    std::vector<int> recipes;
public:
    Kitchen()
        : recipes{std::vector<int>{3, 7}} { /* no reserve.. */ }

    long cook_n(int target);
    size_t cook_till_seq(const std::string& sequence);

private:
    void cook();
    void assign_new_recipes();
};

long Kitchen::cook_n(int target)
{
    size_t range = target + 10;
    while (recipes.size() < range) {
        cook();
        assign_new_recipes();
    }

    std::stringstream ss;
    for (size_t i = target; i < range; ++i)
        ss << recipes[i];
    return std::stol(ss.str());
}

size_t Kitchen::cook_till_seq(const std::string& sequence)
{
    std::vector<int> vseq(sequence.size());
    std::transform(std::begin(sequence), std::end(sequence), std::begin(vseq),
            [](auto ch) { return ch - '0'; });

    size_t dist = 0;

    while (true) {
        for (size_t i = 0; i < vseq.size(); ++i) {
            cook();
            assign_new_recipes();
        }

        auto irec = std::begin(recipes);
        std::advance(irec, dist);

        auto it = std::search(irec, std::end(recipes),
                              std::begin(vseq), std::end(vseq));
        if (it != std::end(recipes))
            return std::distance(std::begin(recipes), it);
        else
            dist = recipes.size() + 1 - vseq.size();
    }
}

void Kitchen::cook()
{
    auto result = recipes[e1] + recipes[e2];
    if (result > 9)
        recipes.push_back(1);
    recipes.push_back(result % 10);
}

inline void Kitchen::assign_new_recipes()
{
    e1 = (e1 + 1 + recipes[e1]) % recipes.size();
    e2 = (e2 + 1 + recipes[e2]) % recipes.size();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 14 - Chocolate Charts\n";

    auto input = utils::get_input_string(argc, argv, "14");
    auto k = Kitchen{};

    auto part1 = k.cook_n(std::stoi(input));
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = k.cook_till_seq(input);
    std::cout << "Part 2: " << part2 << '\n';
}
