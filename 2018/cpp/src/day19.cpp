#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <sstream>
#include <unordered_map>
#include <functional>

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
    void run_program(int reg_zero = 0);
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

void Device::run_program(int reg_zero)
{
    std::fill(std::begin(regs), std::end(regs), 0);     // reset registers

    regs[0] = reg_zero;
    while (regs[ipr] < int(program.size())) {
        ops.execute(program[regs[ipr]], regs);
        ++regs[ipr];
    }
}

int get_part2()
{
    //const int r4 = 947;               // part 1 value
    const int r4 = 10'551'347;          // calculated by hand
    int r0 = 0;

    for (int r1 = 1; r1 <= r4; ++r1)
        if (r4 % r1 == 0)
            r0 += r1;
    return r0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 19 - Go With The Flow\n";

    auto input = utils::get_input_string(argc, argv, "19");

    Device device;
    device.load_program(input);
    device.run_program();

    auto part1 = device.get_register_value(0);
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = get_part2();
    std::cout << "Part 2: " << part2 << '\n';
}
