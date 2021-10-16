#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include <regex>

#include <get_input.hpp>

struct Step {
    char prereq;
    char step_token;
};

std::ostream& operator<<(std::ostream& os, const Step& s)
{
    return os << "Do " << s.prereq << " before " << s.step_token;
}

auto parse_steps(const std::vector<std::string>& input)
{
    std::vector<Step> vsteps (input.size());

    const std::regex pat {R"(^Step\s(\w).*(\w)\scan\sbegin\.$)"};

    std::transform(std::begin(input), std::end(input), std::begin(vsteps),
            [&pat](auto line) {
                //std::cmatch match;
                std::smatch match;
                std::regex_search(line, match, pat);
                //Step step {match[1], match[2]};
                Step step {match[1].str()[0], match[2].str()[0]};

                return step;
            });

    return vsteps;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Worker {
    char task = '\0';
    int time_left = 0;
    void dec_time() { --time_left; }
};

class Task_mgr {
public:
    Task_mgr(size_t max_w, size_t num_s, int base_t = 0);

    std::string get_order_string() const;

    int  run(const std::vector<Step>& steps);

private:
    int  get_task_time(char t) const;
    bool tasks_complete() const { return done.size() == num_steps; }

    void assign_task(char t);
    void advance_time();
    bool check_workers();
    void review_tasks(const std::vector<Step>& steps);

    const char    first_token = 'A';
    const size_t  max_workers;  // max workers
    const size_t  num_steps;    // number of tasks
    const int     base_tt;      // base task time
    std::vector<char> done;
    std::vector<char> ready;
    std::vector<char> pool;
    std::list<Worker> workers;
};

Task_mgr::Task_mgr(size_t max_w, size_t num_s, int base_t)
    : max_workers{max_w},
      num_steps{num_s},
      base_tt{base_t},
      pool{std::vector<char>(num_s)}
{
    std::iota(std::begin(pool), std::end(pool), first_token);
}

int Task_mgr::get_task_time(char task) const
{
    return base_tt + 1 + task - first_token;
}

void Task_mgr::assign_task(char t)
{
    workers.push_back(Worker{t, get_task_time(t)});
}

void Task_mgr::advance_time()
{
    std::for_each(std::begin(workers), std::end(workers),
            [](auto& w) { w.dec_time(); });
}

bool Task_mgr::check_workers()
{
    if (workers.size() == 0)
        return true;

    bool change = false;
    for (auto it = std::begin(workers); it != std::end(workers); )
        if (it->time_left <= 0) {
            done.push_back(it->task);
            change = true;
            it = workers.erase(it);
        } else {
            ++it;
        }
    return change;
}

void Task_mgr::review_tasks(const std::vector<Step>& steps)
{
    ready.resize(pool.size());
    std::copy(std::begin(pool), std::end(pool), std::begin(ready));
    pool.clear();

    for (const auto& s : steps) {
        auto dit = std::find(std::begin(done), std::end(done), s.prereq);
        if (dit == std::end(done)) {
            auto rit = std::find(std::begin(ready), std::end(ready),
                                 s.step_token);
            if (rit != std::end(ready)) {
                ready.erase(rit);
                pool.push_back(s.step_token);
            }
        }
    }

    std::sort(std::rbegin(ready), std::rend(ready));
    for (auto it = std::rbegin(ready);
            it != std::rend(ready) && workers.size() < max_workers; ) {
        assign_task(*it);
        ++it;
        ready.pop_back();
    }
    for (const auto c : ready)
        pool.push_back(c);
    ready.clear();
}

int Task_mgr::run(const std::vector<Step>& steps)
{
    int total_time = 0;

    while (!tasks_complete()) {
        if (check_workers())
            review_tasks(steps);

        if (!tasks_complete()) {
            advance_time();
            ++total_time;
        }
    }

    return total_time;
}

std::string Task_mgr::get_order_string() const
{
    return std::string{std::begin(done), std::end(done)};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 7 - The Sum of Its Parts\n";

    auto input = utils::get_input_lines(argc, argv, "07");
    auto steps = parse_steps(input);

    Task_mgr solo_mgr {1, 26};
    auto time1 = solo_mgr.run(steps);
    auto order1 = solo_mgr.get_order_string();
    std::cout << "Part 1: " << time1 << "s " << order1 << '\n';

    Task_mgr group_mgr {5, 26, 60};
    auto time2 = group_mgr.run(steps);
    auto order2 = group_mgr.get_order_string();
    std::cout << "Part 2: " << time2 << "s " << order2 << '\n';
}
