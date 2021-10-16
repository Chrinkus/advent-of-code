#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>

#include <get_input.hpp>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Growth_condition {
    explicit Growth_condition(const std::string& s);
    std::string pattern;
    bool result;
};

Growth_condition::Growth_condition(const std::string& s)
{
    std::istringstream iss {s};
    iss >> pattern;

    std::string throwaway;
    iss >> throwaway;

    char r;
    iss >> r;
    result = r == '#' ? true : false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Plant_plan {
public:
    Plant_plan(std::string init, std::vector<Growth_condition> vcond)
        : state{std::move(init)}, conditions{std::move(vcond)} { }

    int64_t sum_plant_pots() const;
    const std::string& get_state() const { return state; }

    int64_t advance_year(int64_t n);
private:
    void prep_state();
    void advance_year();

    int zero_offset = 0;
    std::string state;
    std::vector<Growth_condition> conditions;
};

void Plant_plan::prep_state()
{
    const int dist_const = 4;

    auto it = std::find(std::begin(state), std::end(state), '#');
    int dist = std::distance(std::begin(state), it);
    if (dist < dist_const) {
        state.insert(0, dist_const - dist, '.');
        zero_offset += dist_const - dist;
    } else {
        state.erase(0, dist - dist_const);
        zero_offset -= dist - dist_const;
    }

    auto rit = std::find(std::rbegin(state), std::rend(state), '#');
    if ((dist = std::distance(std::rbegin(state), rit)) < dist_const) {
        state.append(dist_const - dist, '.');
    }
}

void Plant_plan::advance_year()
{
    prep_state();

    std::string next_year(state.size(), '.');
    for (size_t i = 0; i < state.size() - 5; ++i) {

        auto cit = std::find_if(std::begin(conditions), std::end(conditions),
                [this, i](const auto& cnd) {
                    return cnd.pattern == this->state.substr(i, 5);
                });

        if (cit->result)
            next_year[i + 2] = '#';
    }
    state = next_year;
}

int64_t Plant_plan::advance_year(int64_t n)
{
    int64_t this_year, last_year, this_diff, last_diff = -1;
    this_year = last_year = this_diff = 0;

    while (this_diff != last_diff && n--) {
        advance_year();
        last_year = this_year;
        this_year = sum_plant_pots();
        last_diff = this_diff;
        this_diff = this_year - last_year;
    }

    int64_t total = sum_plant_pots();
    while (n-- > 0)
        total += this_diff;
    return total;
}

int64_t Plant_plan::sum_plant_pots() const
    // not sure how to use std::accumulate here. need 'i'.
{
    int64_t total = 0;
    for (size_t i = 0; i < state.size(); ++i)
        if (state[i] == '#')
            total += i - zero_offset;
    return total;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

auto parse_input(const std::vector<std::string>& input)
{
    const std::regex state_pat {R"(^initial state: ([#.]+)$)"};
    const std::regex growth_pat {R"(^([#.]{5} => (#|\.)$))"};
    std::smatch match;

    auto it = std::begin(input);

    std::string state;
    if (std::regex_match(*it++, match, state_pat)) 
        state = match[1];
    else
        std::cout << "Could not read initial state\n";

    std::vector<Growth_condition> vcond;
    while (++it != std::end(input)) {
        if (std::regex_match(*it, match, growth_pat))
            vcond.emplace_back(Growth_condition{match[1].str()});
        else
            std::cout << "Could not read growth pattern\n";
    }

    return Plant_plan{state, vcond};
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 12 - Subterranean Sustainability\n";

    auto input = utils::get_input_lines(argc, argv, "12");

    Plant_plan plan1 = parse_input(input);
    auto part1 = plan1.advance_year(20);
    std::cout << "Part 1: " << part1 << '\n';

    Plant_plan plan2 = parse_input(input);
    auto part2 = plan2.advance_year(50000000000);
    std::cout << "Part 2: " << part2 << '\n';
}

