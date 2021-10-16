#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <sstream>

#include <get_input.hpp>

using Registers = std::array<int,4>;
using Instruction = std::array<int,4>;
using Execution = std::function<Registers(const Instruction&,Registers)>;

struct Sample_op {
public:
    Registers before;
    Registers after;
    Instruction ins;
};

std::ostream& operator<<(std::ostream& os, const Sample_op& so)
{
    os << "Before: [ ";
    for (const auto v : so.before)
        os << v << ' ';
    os << "] Inst: ";
    for (const auto v : so.ins)
        os << v << ' ';
    os << "After: [";
    for (const auto v : so.after)
        os << v << ' ';
    return os << ']';
}

std::istream& operator>>(std::istream& is, Sample_op& so)
{
    char ch;
    is >> ch;
    if (ch != 'B') {
        is.putback(ch);
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string bef_aft;
    is >> bef_aft >> ch;
    for (auto& v : so.before)
        is >> v >> ch;

    for (auto& v : so.ins)
        is >> v;

    is >> bef_aft >> ch;
    for (auto& v : so.after)
        is >> v >> ch;

    return is;
}

auto parse_input(std::istringstream& iss)
{
    std::vector<Sample_op> vso;

    for (Sample_op so; iss >> so; )
        vso.push_back(so);

    if (iss.fail()) {               // it should, there are two parts to input
        iss.clear();
    }

    return vso;
}

class Operations {
private:
    std::unordered_map<std::string,Execution> ops;
    std::unordered_map<std::string,std::vector<int>> op_tally;
    std::unordered_map<int,std::string> op_map;
public:
    Operations();

    int count_possible(const Sample_op& s) const;
    void map_ops(const std::vector<Sample_op>& vso);
    void print_op_tally() const;
    void execute(const Instruction& ins, Registers& r) const;
private:
    void reduce_op_tally(int key);
};

Operations::Operations()
{
    ops.emplace("addr", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] + r[ins[2]];
                return r;
            });
    ops.emplace("addi", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] + ins[2];
                return r;
            });
    ops.emplace("mulr", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] * r[ins[2]];
                return r;
            });
    ops.emplace("muli", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] * ins[2];
                return r;
            });
    ops.emplace("banr", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] & r[ins[2]];
                return r;
            });
    ops.emplace("bani", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] & ins[2];
                return r;
            });
    ops.emplace("borr", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] | r[ins[2]];
                return r;
            });
    ops.emplace("bori", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]] | ins[2];
                return r;
            });
    ops.emplace("setr", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = r[ins[1]];
                return r;
            });
    ops.emplace("seti", [](const Instruction& ins, Registers r) {
                r[ins[3]]  = ins[1];
                return r;
            });
    ops.emplace("gtir", [](const Instruction& ins, Registers r) {
                r[ins[3]] = ins[1] > r[ins[2]] ? 1 : 0;
                return r;
            });
    ops.emplace("gtri", [](const Instruction& ins, Registers r) {
                r[ins[3]] = r[ins[1]] > ins[2] ? 1 : 0;
                return r;
            });
    ops.emplace("gtrr", [](const Instruction& ins, Registers r) {
                r[ins[3]] = r[ins[1]] > r[ins[2]] ? 1 : 0;
                return r;
            });
    ops.emplace("eqir", [](const Instruction& ins, Registers r) {
                r[ins[3]] = ins[1] == r[ins[2]] ? 1 : 0;
                return r;
            });
    ops.emplace("eqri", [](const Instruction& ins, Registers r) {
                r[ins[3]] = r[ins[1]] == ins[2] ? 1 : 0;
                return r;
            });
    ops.emplace("eqrr", [](const Instruction& ins, Registers r) {
                r[ins[3]] = r[ins[1]] == r[ins[2]] ? 1 : 0;
                return r;
            });
}

int Operations::count_possible(const Sample_op& s) const
{
    auto t = std::count_if(std::begin(ops), std::end(ops),
            [&s](const auto& op) {
                auto after = op.second(s.ins, s.before);
                return after == s.after;
            });

    return t;
}

void Operations::map_ops(const std::vector<Sample_op>& vso)
{
    for (const auto& op : ops) {
        std::vector<int> codes;
        for (const auto& so : vso) {
            auto code = so.ins[0];
            auto it = std::find(std::begin(codes), std::end(codes), code);

            if (op.second(so.ins, so.before) == so.after) {
                if (it == std::end(codes))
                    codes.push_back(code);
            } else {
                if (it != std::end(codes))
                    codes.erase(it);
            }
        }
        op_tally[op.first] = std::move(codes);
    }

    auto key = std::find_if(std::begin(op_tally), std::end(op_tally),
            [](auto& op) { return op.second.size() == 1; })->second.front();

    reduce_op_tally(key);

    for (const auto& op : op_tally)
        op_map[op.second.front()] = op.first;
}

void Operations::reduce_op_tally(int key)
{
    for (auto& op : op_tally) {
        auto it = std::find(std::begin(op.second), std::end(op.second), key);
        if (op.second.size() > 1 && it != std::end(op.second)) {
            op.second.erase(it);
            if (op.second.size() == 1)
                reduce_op_tally(op.second.front());
        }
    }
}

void Operations::print_op_tally() const
{
    for (const auto& op : op_tally) {
        std::cout << op.first << '\t';
        for (const auto& i : op.second)
            std::cout << i << ' ';
        std::cout << '\n';
    }
}

void Operations::execute(const Instruction& ins, Registers& r) const
{
    r = ops.at(op_map.at(ins[0]))(ins, r);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int get_part_one(const Operations& ops, const std::vector<Sample_op>& vso)
{
    std::vector<int> vcounts;
    for (const auto& so : vso)
        vcounts.push_back(ops.count_possible(so));

    return std::count_if(std::begin(vcounts), std::end(vcounts),
            [](const auto c) { return c >= 3; });
}

int run_test_program(std::istream& is, const Operations& ops)
{
    Registers r { 0, 0, 0, 0 };
    std::string line;
    while (is) {
        Instruction ins;
        for (auto& v : ins)
            is >> v;
        ops.execute(ins, r);
    }
    return r[0];
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 16 Chronal Classification\n";

    auto input = utils::get_input_string(argc, argv, "16");

    std::istringstream iss {input};
    auto vso = parse_input(iss);
    Operations ops {};
    ops.map_ops(vso);

    auto part1 = get_part_one(ops, vso);
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = run_test_program(iss, ops);
    std::cout << "Part 2: " << part2 << '\n';
}
