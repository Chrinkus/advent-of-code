#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <regex>

#include <get_input.hpp>

struct Step {
    char do_this = '\0';
    char before = '\0';
};

std::ostream& operator<<(std::ostream& os, const Step& s)
{
    return os << "Do " << s.do_this << " before " << s.before;
}

auto parse_steps(const std::vector<std::string>& input)
{
    std::vector<Step> vsteps (input.size());

    std::transform(std::begin(input), std::end(input), std::begin(vsteps),
            [](auto line) {
                static const std::regex pat_1 {R"(^Step\s(\w))"};
                static const std::regex pat_2 {R"((\w) can begin\.\s?$)"};
                Step step;
                std::smatch m_1;
                std::regex_search(line, m_1, pat_1);
                step.do_this = m_1[1].str()[0];
                std::smatch m_2;
                std::regex_search(line, m_2, pat_2);
                step.before = m_2[1].str()[0];

                return step;
            });

    return vsteps;
}

/*
auto get_step_order(const std::vector<Step>& steps)
{
    std::vector<char> done;
    std::vector<char> ready;
    std::vector<char> pool;
    for (char c = 'A'; c <= 'Z'; ++c)
        pool.push_back(c);

    while (pool.size()) {
        ready.resize(pool.size());
        std::copy(std::begin(pool), std::end(pool), std::begin(ready));
        pool.clear();

        for (const auto& step : steps) {
            auto dit = std::find(std::begin(done), std::end(done),
                                 step.do_this);     // check if do_this is done
            if (dit == std::end(done)) {            // not done
                auto rit = std::find(std::begin(ready), std::end(ready),
                                     step.before);
                if (rit != std::end(ready)) {
                    ready.erase(rit);
                    pool.push_back(step.before);
                }
            }
        }
        std::sort(std::rbegin(ready), std::rend(ready));
        done.push_back(ready.back());
        ready.pop_back();
        for (const auto c : ready)
            pool.push_back(c);
        ready.clear();
    }
    return std::string {std::begin(done), std::end(done)};
}
*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Worker {
    char task = '\0';
    int time_left = 0;
};

class Task_mgr {
public:
    Task_mgr(size_t max_workers, char begin_t, char end_t,
             int base_task_time = 0);

    int  run(const std::vector<Step>& steps);
    std::string get_order_string() const;

private:
    int  get_task_time(char t) const;
    bool tasks_complete() const { return done.size() == num_tasks(); }
    void assign_task(char t);
    void advance_time();
    bool check_workers();
    void review_tasks(const std::vector<Step>& steps);
    size_t num_tasks() const { return l_tok - f_tok + 1; }

    const size_t  max_w;        // max workers
    const char f_tok;           // first token
    const char l_tok;           // last token
    const int  btt;             // base task time
    std::vector<char> done;
    std::deque<char> ready;
    std::vector<char> pool;
    std::list<Worker> workers;
};

Task_mgr::Task_mgr(size_t max_workers, char begin_t, char end_t,
                   int base_task_time)
    : max_w{max_workers}, f_tok{begin_t}, l_tok{end_t}, btt{base_task_time}
{
    for (char task = f_tok; task <= l_tok; ++task)
        pool.push_back(task);
}

int Task_mgr::get_task_time(char task) const
{
    return btt + 1 + task - f_tok;
}

void Task_mgr::assign_task(char t)
{
    workers.push_back(Worker{t, get_task_time(t)});
}

void Task_mgr::advance_time()
{
    std::for_each(std::begin(workers), std::end(workers),
            [](auto& w) { if (w.time_left > 0) --w.time_left; });
}

// add to class
bool Task_mgr::check_workers()
{
    if (workers.size() == 0)
        return true;

    bool change = false;
    for (auto it = std::begin(workers); it != std::end(workers); )
        if (it->time_left == 0) {
            done.push_back(it->task);
            change = true;
            it = workers.erase(it);
        } else {
            ++it;
        }
    return change;
}

/* Deque attempt
void Task_mgr::load_ready(const std::vector<Step>& steps)
    // put all tasks that have no pre-requisites into ready
{
    ready.resize(pool.size());
    std::copy(std::begin(pool), std::end(pool), std::begin(ready));
    pool.clear();

    for (const auto& s : steps) {
        auto dit = std::find(std::begin(done), std::end(done), s.do_this);
        if (dit == std::end(done)) {
            auto rit = std::find(std::begin(ready), std::end(ready), s.before);
            if (rit != std::end(ready)) {
                ready.erase(rit);
                pool.push_back(s.before);
            }
        }
    }
    std::sort(std::begin(ready), std::end(ready));
}
*/

void Task_mgr::review_tasks(const std::vector<Step>& steps)
{
    ready.resize(pool.size());
    std::copy(std::begin(pool), std::end(pool), std::begin(ready));
    pool.clear();

    for (const auto& s : steps) {
        auto dit = std::find(std::begin(done), std::end(done), s.do_this);
        if (dit == std::end(done)) {
            auto rit = std::find(std::begin(ready), std::end(ready), s.before);
            if (rit != std::end(ready)) {
                ready.erase(rit);
                pool.push_back(s.before);
            }
        }
    }

    /* Deque attempt
    std::sort(std::begin(ready), std::end(ready));
    for (auto it = std::begin(ready),
            it != std::end(ready) && workers.size() < max_w; ) {
        assign_task(*it++);
        ready.pop_front();
    }
    */
    std::sort(std::rbegin(ready), std::rend(ready));
    for (auto it = std::rbegin(ready);
            it != std::rend(ready) && workers.size() < max_w; ) {
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

int main()
{
    std::cout << "AoC 2018 Day 7 - The Sum of Its Parts\n";

    auto input = utils::get_input_lines();
    auto steps = parse_steps(input);

    Task_mgr solo_mgr {1, 'A', 'Z'};
    auto time1 = solo_mgr.run(steps);
    auto order1 = solo_mgr.get_order_string();
    std::cout << "Part 1: " << time1 << "s " << order1 << '\n';

    Task_mgr group_mgr {5, 'A', 'Z', 60};
    auto time2 = group_mgr.run(steps);
    auto order2 = group_mgr.get_order_string();
    std::cout << "Part 2: " << time2 << "s " << order2 << '\n';
}
