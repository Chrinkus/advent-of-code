#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std::chrono;

class Timer {
    using Clock = std::chrono::steady_clock;
    using Time_point = std::chrono::time_point<Clock>;
public:
    Timer(const bool start_now = true) { if (start_now) start(); }

    void start() { start_stamp = Clock::now(); }
    void end(const int width) { stamp("Time"); print_log(width); }

    void stamp(const std::string& label)
    {
        log.push_back(std::make_pair(label, Clock::now()));
    }

    void print_log(const int width = 8) const;

private:

    Time_point start_stamp;
    std::vector<std::pair<std::string,Time_point>> log;
};

void Timer::print_log(const int width) const
{
    for (const auto& [ l, t ] : log) {

        std::cout << std::left << std::setw(width) << l << ":\t"
                  << duration<double, std::milli>(t - start_stamp).count()
                  << " ms\n";
    }
}