#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <memory>

#include <get_input.hpp>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Instruction {
    std::string name;
    std::array<int,3> fields;
};

std::istream& operator>>(std::istream& is, Instruction& ins)
{
    is >> ins.name;
    for (auto& f : ins.fields)
        is >> f;
    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

using Registers = std::array<int,6>;
using Execution = std::function<void(const Instruction&,Registers&)>;

class Operations {
private:
    std::unordered_map<std::string,Execution> ops;
public:
    Operations();

    void execute(const Instruction& ins, Registers& r) const;
};

Operations::Operations()
{
    ops.emplace("addr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] + r[ins.fields[1]];
            });
    ops.emplace("addi", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] + ins.fields[1];
            });
    ops.emplace("mulr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] * r[ins.fields[1]];
            });
    ops.emplace("muli", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] * ins.fields[1];
            });
    ops.emplace("banr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] & r[ins.fields[1]];
            });
    ops.emplace("bani", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] & ins.fields[1];
            });
    ops.emplace("borr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] | r[ins.fields[1]];
            });
    ops.emplace("bori", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]] | ins.fields[1];
            });
    ops.emplace("setr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = r[ins.fields[0]];
            });
    ops.emplace("seti", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]]  = ins.fields[0];
            });
    ops.emplace("gtir", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = ins.fields[0] > r[ins.fields[1]] ? 1 : 0;
            });
    ops.emplace("gtri", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = r[ins.fields[0]] > ins.fields[1] ? 1 : 0;
            });
    ops.emplace("gtrr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = r[ins.fields[0]] > r[ins.fields[1]] ? 1 : 0;
            });
    ops.emplace("eqir", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = ins.fields[0] == r[ins.fields[1]] ? 1 : 0;
            });
    ops.emplace("eqri", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = r[ins.fields[0]] == ins.fields[1] ? 1 : 0;
            });
    ops.emplace("eqrr", [](const Instruction& ins, Registers& r) {
                r[ins.fields[2]] = r[ins.fields[0]] == r[ins.fields[1]] ? 1 : 0;
            });
}

void Operations::execute(const Instruction& ins, Registers& r) const
{
    ops.at(ins.name)(ins, r);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Device {
private:
    Registers regs;
    int ipr;                                // instruction pointer location
    std::vector<Instruction> program;
    Operations ops;
public:
    Device() = default;

    int get_register_value(int reg) const { return regs[reg]; }

    void load_program(const std::string& input);
    int run_program(std::function<int(Registers&,const int)> before);
};

void Device::load_program(const std::string& input)
{
    std::istringstream iss {input};

    std::string ip;
    iss >> ip >> ipr;

    for (Instruction ins; iss >> ins; ) {
        program.push_back(ins);
    }
}

int Device::run_program(std::function<int(Registers&,const int)> before)
{
    std::fill(std::begin(regs), std::end(regs), 0);     // reset registers

    int result = 0;
    while (regs[ipr] < int(program.size())) {
        result = before(regs, ipr);
        ops.execute(program[regs[ipr]], regs);
        ++regs[ipr];
    }
    return result;
}

class Earliest_halt {               // Part 1 functor
public:
    Earliest_halt() = default;

    int operator()(Registers& regs, const int ipr)
    {
        if (regs[ipr] == 16)
            regs[0] = regs[1];
        return regs[0];
    }
};

class Last_halt {                   // Part 2 functor
private:
    std::vector<int> vr0s;
public:
    Last_halt() = default;

    int operator()(Registers& regs, const int ipr)
    {
        if (regs[ipr] == 16) {
            int r0 = regs[1];

            auto it = std::find(std::begin(vr0s), std::end(vr0s), r0);
            if (it != std::end(vr0s))
                regs[0] = r0;
            else
                vr0s.push_back(r0);

        }
        return vr0s.empty() ? 0 : vr0s.back();
    }
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 21 - Chronal Conversion\n";

    auto input = utils::get_input_string(argc, argv, "21");

    Device device;
    device.load_program(input);

    auto part1 = device.run_program(Earliest_halt{});
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = device.run_program(Last_halt{});
    std::cout << "Part 2: " << part2 << '\n';
}
