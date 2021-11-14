#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <array>
#include <thread>

enum class Ins { acc, jmp, nop, err };

using Instructions = std::vector<std::pair<Ins,int>>;

constexpr std::array<std::string_view,3> ins_names { "acc", "jmp", "nop" };

Ins get_instruction(const std::string& s)
{
    auto it = std::find(std::begin(ins_names), std::end(ins_names), s);
    if (it == std::end(ins_names))
        return Ins::err;

    return static_cast<Ins>(std::distance(std::begin(ins_names), it));
}

Instructions read_input()
{
    Instructions vsi;
    std::string ins_s;
    int arg;
    while (std::cin >> ins_s >> arg) {
        auto ins = get_instruction(ins_s);
        vsi.push_back(std::make_pair(ins, arg));
    }
    return vsi;
}

void execute(const Ins op, const int arg, int& accum, std::size_t& index)
{
    switch (op) {
        case Ins::acc:
            accum += arg;
            ++index;
            break;
        case Ins::jmp:
            index += arg;
            break;
        case Ins::nop:
            ++index;
            break;
        case Ins::err:
            std::cerr << "Something's wrong..\n";
            break;
    }
}

std::pair<bool,int> run_instructions(const Instructions& ins)
{
    bool term = true;
    int accum = 0;
    std::vector<std::size_t> seen;
    for (std::size_t i = 0; i < ins.size(); ) {
        // part 1 : track seen instructions
        auto it = std::find(std::begin(seen), std::end(seen), i);
        if (it != std::end(seen)) {
            term = false;
            return std::make_pair(term, accum);
        }
        seen.push_back(i);

        // actually execute instruction
        const auto& [ op, arg ] = ins[i];
        execute(op, arg, accum, i);
    }
    return std::make_pair(term, accum);
}

void wrapper(const Instructions& ins, int& outvar)
{
    auto [ term, accum ] = run_instructions(ins);
    if (term)
        outvar = accum;
}

int spin_em_up(const Instructions& ins)
{
    // identify all indexes to change one way or the other
    std::vector<std::size_t> nop_is;
    std::vector<std::size_t> jmp_is;
    for (std::size_t i = 0; i < ins.size(); ++i) {
        auto [ op, arg ] = ins[i];
        if (op == Ins::jmp)
            jmp_is.push_back(i);
        else if (op == Ins::nop)
            nop_is.push_back(i);
    }

    // spin up threads
    //std::vector<std::thread> vt;
    auto res = 0;
    for (const auto& i : nop_is) {
        Instructions l_ins = ins;
        l_ins[i].first = Ins::jmp;
        wrapper(l_ins, res);
        if (res > 0) return res;
        //vt.push_back(std::thread{wrapper, std::cref(l_ins), std::ref(res)});
    }
    for (const auto& i : jmp_is) {
        Instructions l_ins = ins;
        l_ins[i].first = Ins::nop;
        wrapper(l_ins, res);
        if (res > 0) return res;
        //vt.push_back(std::thread{wrapper, std::cref(l_ins), std::ref(res)});
    }

    /*
    for (auto& t : vt)
        t.join();
    */

    return res;
}


int main()
{
    // Timing start
    auto start = std::chrono::high_resolution_clock::now();

    const auto input = read_input();

    const auto part1 = run_instructions(input).second;
    const auto part2 = spin_em_up(input);
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';

    // Timing end and print
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double,std::micro> time = end - start;
    std::cout << "Time: " << time.count() << " micro-seconds\n";
}
