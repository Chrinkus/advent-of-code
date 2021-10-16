#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <regex>
#include <memory>

#include <get_input.hpp>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// I avoided making this a class for so long. Got part 1 done without it but
// code was so unorganized and repetitive I needed to go a little more OO.
// Part 2 was completed with this as a struct and all members public. Although
// it worked I was embarassed at the lack of encapsulation.

class Guard {
public:
    explicit Guard(int i)
        : id{i}, mins{std::vector<int>(60)}
    {
        std::fill(std::begin(mins), std::end(mins), 0);
    }

    auto get_aoc_answer()  const -> int { return id * sleepy_min; }
    auto get_id()          const -> int { return id; }
    auto get_total_sleep() const -> int { return total_sleep; }
    auto get_sleepy_min()  const -> int { return sleepy_min; }
    auto get_mins()        const -> const std::vector<int>& { return mins; }

    void add_sleep_record(int sl, int aw);
    void calc_total_sleep();
    void map_minutes();
    void set_sleepy_min();
private:
    int id = 0;
    int total_sleep = 0;
    int sleepy_min = -1;
    std::vector<std::pair<int,int>> asleep_awake;
    std::vector<int> mins;
};

void Guard::add_sleep_record(int sl, int aw)
{
    asleep_awake.emplace_back(std::make_pair(sl, aw));
}

void Guard::calc_total_sleep()
{
    total_sleep = std::accumulate(std::begin(asleep_awake),
                                  std::end(asleep_awake),
                                  0,
                                  [](int sum, auto aa) {
                                      return sum + aa.second - aa.first;
                                  });
}

void Guard::map_minutes()
{
    for (const auto& aa : asleep_awake) 
        for (int i = aa.first; i < aa.second; ++i)
            ++(mins[i]);
}

void Guard::set_sleepy_min()
{
    auto it = std::max_element(std::begin(mins), std::end(mins));
    sleepy_min = std::distance(std::begin(mins), it);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// This should also be a class, a wrapper for a hash table and the problem
// specific operations I need for it. I may refactor this again if I have time
// in the future...

using Guard_sleep_map = std::unordered_map<int, std::unique_ptr<Guard>>;

auto retrieve_guard(int key, Guard_sleep_map& gsm)
    // The Chandler Carruth unordered_map access pattern!
{
    auto& pg = gsm[key];
    if (pg)
        return pg.get();
    pg = std::make_unique<Guard>(key);
    return pg.get();
}

void calc_sleep_totals(const Guard_sleep_map& gsm)
{
    for (auto& g : gsm)
        g.second->calc_total_sleep();
}

void map_all_mins(const Guard_sleep_map& gsm)
{
    std::for_each(std::begin(gsm), std::end(gsm),
            [](auto& gp) { gp.second->map_minutes(); });
}

void set_all_sleepy_mins(const Guard_sleep_map& gsm)
{
    std::for_each(std::begin(gsm), std::end(gsm),
            [](auto& gp) { gp.second->set_sleepy_min(); });
}

auto map_sleep_data(const std::vector<std::string>& vs)
    // This is why I want to refactor Guard_sleep_map into its own class. This
    // function is more like a factory/builder function than whatever it is
    // right now.
{
    Guard_sleep_map gsm;

    static const std::regex id_pat {R"(Guard\s#(\d+))"};
    static const std::regex ts_pat {R"(:(\d{2}))"};
    std::smatch matches;
    
    for (auto it = std::begin(vs); it != std::end(vs); ) {
        int id, sleep, wake;
        std::regex_search(*it, matches, id_pat);
        id = std::stoi(matches[1]);
        auto pg = retrieve_guard(id, gsm);

        ++it;
        while (it != std::end(vs) && !std::regex_search(*it, matches, id_pat)) {
            std::regex_search(*it++, matches, ts_pat);
            sleep = std::stoi(matches[1]);
            std::regex_search(*it++, matches, ts_pat);
            wake = std::stoi(matches[1]);
            pg->add_sleep_record(sleep, wake);
        }
    }
    calc_sleep_totals(gsm);
    map_all_mins(gsm);
    set_all_sleepy_mins(gsm);
    return gsm;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Free-standing functions to retrieve the data needed to answer the parts of
// the challenges. The return values are quite awkward and I currently don't
// know what's best to give back, a ref, value or pointer..
// UPDATE: In accordance with the Core Guidelines I am returning a const ref.

const auto& max_sleeper(const Guard_sleep_map& gsm)
{
    auto it = std::max_element(std::begin(gsm), std::end(gsm),
                               [](auto& ga, auto& gb) {
                                   return ga.second->get_total_sleep() <
                                          gb.second->get_total_sleep();
                               });
    return *it->second;
}

// Almost resorted to a vector of three-int-tuples but went the Kate Gregory
// route and made my own struct. I like it.
struct Sleeper {
    Sleeper(int i, int m, int s) : id{i}, minute{m}, sleep{s} { }
    int id = 0;
    int minute = 0;
    int sleep = 0;
};

const auto& find_most_reg_sleeper(const Guard_sleep_map& gsm)
{
    std::vector<Sleeper> vs;

    for (const auto& gp : gsm) {
        auto& p = gp.second;
        auto it = std::max_element(std::begin(p->get_mins()),
                                   std::end(p->get_mins()));
        auto minute = std::distance(std::begin(p->get_mins()), it);
        vs.emplace_back(Sleeper{ p->get_id(), static_cast<int>(minute), *it });
    }

    auto it = std::max_element(std::begin(vs), std::end(vs),
            [](auto& a, auto& b) {
                return a.sleep < b.sleep;
            });

    return *gsm.at(it->id);    // at at at... for const access~!
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 4 - Repose Record\n";

    auto input = utils::get_input_lines(argc, argv, "04");
    std::sort(std::begin(input), std::end(input));

    auto gsm = map_sleep_data(input);

    const auto& worst_guard = max_sleeper(gsm);
    auto part1 = worst_guard.get_aoc_answer();
    std::cout << "Part 1: " << part1 << '\n';

    const auto& reg_sleeper = find_most_reg_sleeper(gsm);
    auto part2 = reg_sleeper.get_aoc_answer();
    std::cout << "Part 2: " << part2 << '\n';
}
