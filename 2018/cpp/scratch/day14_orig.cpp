#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <get_input.hpp>

const size_t big_size = 6'000'000;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Elf {
public:
    explicit Elf(size_t index) : curr_i{index} { }

    void set_next_recipe(const std::vector<int>& recipes);
    size_t get_curr_i() const { return curr_i; }
private:
    size_t curr_i;
};

inline void Elf::set_next_recipe(const std::vector<int>& recipes)
{
    curr_i = (curr_i + 1 + recipes[curr_i]) % recipes.size();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Kitchen {
private:
    Elf orvar;
    Elf tilda;
    std::vector<int> recipes;

public:
    Kitchen()
        : orvar{0}, tilda{1}, recipes{std::vector<int>{ 3, 7 }}
    {
        recipes.reserve(big_size);
    }

    void run(size_t target);
    size_t get_next_10_scores(size_t from);
    size_t r_till_seq(size_t sequence);

    void print() const;

private:
    void cook();
    void assign_new_recipes();
};

void Kitchen::print() const
{
    for (const auto r : recipes)
        std::cout << r << ' ';
    std::cout << '\n';
}

void Kitchen::run(size_t target)
{
    while (recipes.size() < target + 10) {
        cook();
        assign_new_recipes();
    }
}

size_t Kitchen::get_next_10_scores(size_t from)
{
    std::stringstream ss;
    for (size_t i = from; i < from + 10; ++i)
        ss << recipes[i];
    return std::stol(ss.str());
}

size_t Kitchen::r_till_seq(size_t sequence)
{
    std::stringstream ss;
    ss << sequence;
    std::string str_seq = ss.str();
    std::vector<int> vseq (str_seq.size());

    std::transform(std::begin(str_seq), std::end(str_seq), std::begin(vseq),
            [](auto ch) { return ch - '0'; });

    while (true) {
        do {
            cook();
            assign_new_recipes();
        } while (recipes.size() < recipes.capacity());

        auto it = std::search(std::begin(recipes), std::end(recipes),
                              std::begin(vseq), std::end(vseq));
        if (it != std::end(recipes))
            return std::distance(std::begin(recipes), it);
        std::cout << "reallocation...\n";
        /*
        else
            recipes.reserve(recipes.size() * 2);
            */
    }
    /*
    while (recipes.size() < big_size) {
        cook();
        assign_new_recipes();
    }
    auto it = std::search(std::begin(recipes), std::end(recipes),
                          std::begin(vseq), std::end(vseq));
    if (it != std::end(recipes))
        return std::distance(std::begin(recipes), it);
    else
        return 0;
    */
}

void Kitchen::cook()
{
    auto result = recipes[orvar.get_curr_i()] + recipes[tilda.get_curr_i()];

    if (result >= 10)
        recipes.push_back(1);
    recipes.push_back(result % 10);
}

void Kitchen::assign_new_recipes()
{
    orvar.set_next_recipe(recipes);
    tilda.set_next_recipe(recipes);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 14 - Chocolate Charts\n";

    auto input = utils::get_input_value<size_t>(argc, argv, "14");

    Kitchen k;
    k.run(input);

    auto part1 = k.get_next_10_scores(input);
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = k.r_till_seq(input);
    std::cout << "Part 2: " << part2 << '\n';
}

