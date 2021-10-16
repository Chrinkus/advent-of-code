#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <regex>

#include <get_input.hpp>

template<typename Iter>
void circular_inc(Iter first, Iter last, int inc, Iter& it)
{
    while (inc--) {
        if (it == last)
            it = first;
        ++it;
    }
}

template<typename Iter>
void circular_dec(Iter first, Iter last, int dec, Iter& it)
{
    while (dec++) {
        if (it == first)
            it = last;
        --it;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Game_params {
    explicit Game_params(const std::string& desc);
    int players, marbles;
};

Game_params::Game_params(const std::string& description)
{
    static const std::regex pat {R"(^(\d+).*\s(\d+)\spoints$)"};
    std::smatch matches;
    if (std::regex_match(description, matches, pat)) {
        players = std::stoi(matches[1]);
        marbles = std::stoi(matches[2]);
    } else {
        std::cerr << "Error: could not extract data from text\n";
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

uint64_t play_marbles(int num_players, int marbles)
{
    int marble = 0;
    std::list<int> circle {marble};
    std::vector<uint64_t> scores (num_players, 0);
    size_t player_i = 0;

    auto it = std::begin(circle);
    while (marble < marbles) {
        ++marble;

        if (marble % 23 == 0) {
            circular_dec(std::begin(circle), std::end(circle), -7, it);
            scores[player_i] += marble + *it;
            it = circle.erase(it);
        } else {
            circular_inc(std::begin(circle), std::end(circle), 2, it);
            it = circle.insert(it, marble);
        }

        ++player_i;
        player_i %= num_players;
    }

    return *std::max_element(std::begin(scores), std::end(scores));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 9 - Marble Mania\n";

    auto input = utils::get_input_string(argc, argv, "09");

    auto gp = Game_params{input};

    auto part1 = play_marbles(gp.players, gp.marbles);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = play_marbles(gp.players, gp.marbles * 100);
    std::cout << "Part 2: " << part2 << '\n';
}
