#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>
#include <deque>                // !!!

#include "Timer.hpp"            // for timing solution
constexpr int WIDTH = 5;        // for output alignment

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Input structs
using Index = std::size_t;
using Range = std::pair<int,int>;
constexpr Index SENTRY = SIZE_MAX;

struct Rule {
    std::string field;
    Range r1;
    Range r2;
    Index i = SENTRY;
};

std::ostream& operator<<(std::ostream& os, const Rule& r)
    // for debugging
{
    return os << r.field << ' ' << r.i << ' '
              << r.r1.first << '-' << r.r1.second << ' '
              << r.r2.first << '-' << r.r2.second;
}

struct Ticket {
    std::vector<int> values;
};

auto extract_ticket(const std::string& line)
{
    std::vector<int> vals;
    std::istringstream iss {line};
    char ch;
    for (int n; iss >> n; ) {
        vals.push_back(n);
        iss >> ch;
    }
    return vals;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Input-parsing beast
auto separate_input()
{
    std::vector<Rule> vrules;
    Ticket my_t;
    std::vector<Ticket> vticks;

    static const std::regex rule_pat {
        R"(^([\w ]+): (\d+)-(\d+) or (\d+)-(\d+)$)"
    };

    // Rules
    for (std::string r; std::getline(std::cin, r) && !r.empty(); ) {
        std::smatch match;
        if (std::regex_match(r, match, rule_pat)) {
            Rule rule;
            rule.field = match[1].str();
            auto low = std::stoi(match[2].str());
            auto high = std::stoi(match[3].str());
            rule.r1 = std::make_pair(low, high);
            low = std::stoi(match[4].str());
            high = std::stoi(match[5].str());
            rule.r2 = std::make_pair(low, high);
            vrules.push_back(rule);
        } else {
            std::cerr << "No pattern match: " << r << '\n';
        }
    }

    // My ticket
    std::string xx;
    std::getline(std::cin, xx);     // burn "your ticket:" line
    std::getline(std::cin, xx);
    my_t.values = extract_ticket(xx);

    // Their tickets
    std::getline(std::cin, xx);     // burn empty line
    std::getline(std::cin, xx);     // burn "nearby tickets"
    while (std::getline(std::cin, xx)) {
        Ticket t;
        t.values = extract_ticket(xx);
        vticks.push_back(t);
    }

    return std::make_tuple(vrules, my_t, vticks);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 1
auto get_great_ranges(const std::vector<Rule>& vr)
    // establish widest possible ranges for values
{
    if (vr.empty()) throw;          // ??? correct ???
    auto it = std::begin(vr);
    auto [ lo_1, hi_1 ] = it->r1;
    auto [ lo_2, hi_2 ] = it->r2;
    for (++it; it != std::end(vr); ++it) {
        auto [ l1, h1 ] = it->r1;
        if (l1 < lo_1) lo_1 = l1;
        if (h1 > hi_1) hi_1 = h1;
        auto [ l2, h2 ] = it->r2;
        if (l2 < lo_2) lo_2 = l2;
        if (h2 > hi_2) hi_2 = h2;
    }
    auto lo_r = std::make_pair(lo_1, hi_1);
    auto hi_r = std::make_pair(lo_2, hi_2);
    return std::make_pair(lo_r, hi_r);
}

auto get_error_rate(const std::vector<Rule>& vr, const std::vector<Ticket>& vt)
{
    auto [ gr_lo, gr_hi ] = get_great_ranges(vr);
    auto error_rate = 0;
    std::vector<Ticket> vvt;        // valid vector of Tickets
    const auto [ l1, h1 ] = gr_lo;
    const auto [ l2, h2 ] = gr_hi;
    for (const auto& t : vt) {
        auto valid = true;
        for (const auto v :t.values) {
            if (v < l1 || h2 < v || (h1 < v && v < l2)) {
                error_rate += v;
                valid = false;
            }
        }
        if (valid) vvt.push_back(t);
    }
    return std::make_pair(error_rate, vvt);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 2
auto falls_in_ranges(const int n, const Range& r1, const Range& r2)
{
    if (r1.first <= n && n <= r1.second)
        return true;
    else if (r2.first <= n && n <= r2.second)
        return true;
    return false;
}

auto all_fall(const std::vector<int>& fv, const Rule& r)
{
    for (const auto v : fv) {
        if (!falls_in_ranges(v, r.r1, r.r2)) {
            return false;
        }
    }
    return true;
}

struct Field {
    Index i;
    std::vector<int> vals;
};

auto decipher_fields(std::vector<Rule> vr, const std::vector<Ticket>& vt)
    // fields can match multiple rules, need to eliminate rules with one match
{
    std::deque<Field> df;
    for (Index i = 0; i < vt.front().values.size(); ++i) {
        std::vector<int> field_vals;
        for (Index j = 0; j < vt.size(); ++j)
            field_vals.push_back(vt[j].values[i]);
        df.push_back(Field{i,field_vals});
    }

    while (!df.empty()) {
        auto f = df.front();
        std::vector<Rule*> vrp;
        for (auto& r : vr) {
            if (r.i == SENTRY && all_fall(f.vals, r))
                vrp.push_back(&r);
        }
        if (vrp.size() == 1) {
            vrp.front()->i = f.i;
        } else {
            df.push_back(f);
        }
        df.pop_front();
    }
    return vr;
}

auto read_my_ticket(const Ticket& t, const std::vector<Rule>& vr)
{
    Index res = 1;
    for (const auto& r : vr) {
        if (r.field.find("departure") != std::string::npos)
            res *= t.values[r.i];
    }
    return res;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
try {
    Timer t {};         // start of timing

    const auto [rules,mine,theirs] = separate_input();

    const auto [ part1, valid_tickets ] = get_error_rate(rules, theirs);
    const auto known_rules = decipher_fields(rules, valid_tickets);
    const auto part2 = read_my_ticket(mine, known_rules);

    std::cout << std::setw(WIDTH) << "Part 1: " << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2: " << part2 << '\n';

    t.end(WIDTH);       // end of timing, print report
}
catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
