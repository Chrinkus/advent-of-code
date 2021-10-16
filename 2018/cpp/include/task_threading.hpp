#include <vector>
#include <thread>

namespace utils {

static constexpr size_t MIN_THREADS = 2;

template <typename F, typename C, typename D, typename T>
void task_scheduler(F f, const C& src, D& dest, size_t offset, size_t split,
                    const T& t)
{
    auto dit = std::begin(dest);
    auto sit = std::begin(src);
    std::advance(dit, offset);
    std::advance(sit, offset);

    auto sentry = std::begin(src);
    std::advance(sentry, std::min(offset + split, src.size()));

    while (sit != sentry) {
        *dit = f(t, *sit);
        ++dit;
        ++sit;
    }
}

template <typename F, typename C, typename D, typename T>
void split_task(F task, const C& src, D& dest, const T& t)
{
    if (dest.size() < src.size())   // ensure destination is at least same size
        dest.resize(src.size());

    auto num_threads = std::thread::hardware_concurrency();
    auto split = src.size() / num_threads;
    if (split < MIN_THREADS) {     
        split = MIN_THREADS;
        num_threads = src.size() / MIN_THREADS;
    }
    else if (src.size() % split != 0)   // ensure splits will cover size
        ++split;

    auto sched = task_scheduler<F,C,D,T>;
    std::vector<std::thread> vt (num_threads);
    for (size_t i = 0; i < vt.size(); ++i) {
        size_t offset = i * split;
        vt[i] = std::thread{sched, task, std::cref(src), std::ref(dest),
                            offset, split, std::cref(t)};
    }

    for (auto& t : vt)
        t.join();
}

}   // utils
