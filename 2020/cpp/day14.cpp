#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <bitset>
#include <variant>
#include <unordered_map>
#include <numeric>

#include "Read_input.hpp"       // simple, handy reader
#include "Timer.hpp"            // for timing solution
constexpr int WIDTH = 5;        // for output alignment

using Index = std::size_t;
using Bigint = std::uint64_t;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
constexpr std::uint16_t BITS = 36;

enum class Ins { mask, mem };

struct Mask {
    std::string mask;
};

struct Mem {
    std::bitset<BITS> index = 0;
    std::bitset<BITS> value = 0;
};

struct Instruction {
    Ins ins;
    std::variant<Mask,Mem> data;
};
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
auto parse_input(const std::vector<std::string>& vs)
    // Windows does NOT like this function..
{
    // Magic number in the regex 
    static const std::regex mask_pat {R"(^mask = ([X01]{36})$)"};
    static const std::regex mem_pat {R"(^mem\[(\d+)\] = (\d+)$)"};

    std::vector<Instruction> vins;

    for (const auto& s : vs) {
        Instruction inst{};
        std::smatch match;
        if (std::regex_match(s, match, mem_pat)) {
            inst.ins = Ins::mem;
            auto i = std::stoull(match[1].str());
            auto v = std::stoull(match[2].str());
            Mem mem{i,v};
            inst.data = mem;
        } else if (std::regex_match(s, match, mask_pat)) {
            inst.ins = Ins::mask;
            std::string m = match[1];
            Mask mask{m};
            inst.data = mask;
        } else {
            std::cerr << "No pattern match: " << s << '\n';
        }
        vins.push_back(inst);
    }
    return vins;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Bigint memory_sum(const std::unordered_map<Index,Bigint>& memory)
{
    return std::accumulate(std::begin(memory), std::end(memory), Bigint{},
            [](const auto& a, const auto& b) {
                const auto [i,v] = b;
                return a + v;
            });
}

Bigint decoder_v1(const std::vector<Instruction>& vins)
{
    std::unordered_map<Index,Bigint> memory;
    std::string mask;
    for (const auto& ins : vins) {
        if (ins.ins == Ins::mask) {
            mask = std::get<Mask>(ins.data).mask;
        } else {
            const auto& index = std::get<Mem>(ins.data).index;
            std::bitset<BITS> value = std::get<Mem>(ins.data).value;
            for (Index i = 0, j = mask.size() - 1; i < mask.size(); ++i, --j) {
                switch (mask[i]) {
                case '1':   value[j] = true;    break;
                case '0':   value[j] = false;   break;
                case 'X':                       break;
                }
            }
            memory[index.to_ullong()] = value.to_ullong();
        }
    }
    return memory_sum(memory);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 2
auto generate_bit_variations(std::bitset<BITS> bs, Index i,
        const std::vector<Index>& vi, std::vector<std::bitset<BITS>>& vb)
{
    if (i == vi.size()) {
        vb.push_back(bs);
        return;
    }
    bs[vi[i]] = 0;
    generate_bit_variations(bs, i+1, vi, vb);
    bs[vi[i]] = 1;
    generate_bit_variations(bs, i+1, vi, vb);
}

auto get_all_masks(const std::string& mask_pat)
{
    std::vector<std::bitset<BITS>> vbm;
    // Identify the 'X' bits
    std::vector<Index> vpos;
    for (Index i = 0, j = mask_pat.size() - 1; i < mask_pat.size(); ++i, --j)
        if (mask_pat[i] == 'X')
            vpos.push_back(j);

    generate_bit_variations(std::bitset<BITS>{0}, Index{}, vpos, vbm);

    return vbm;
}

auto prep_addr(std::bitset<BITS> addr, const std::string& mask)
    // if a mask bit is 'X' set the corresponding addr bit to 0
    // if a mask bit is '1' set the corresponding addr bit to 1
{
    for (Index i = 0, j = mask.size() - 1; i < mask.size(); ++i, --j)
        switch (mask[i]) {
        case 'X':   addr[j] = 0;    break;
        case '1':   addr[j] = 1;    break;
        }

    return addr;
}

auto decoder_v2(const std::vector<Instruction>& vins)
{
    std::unordered_map<Index,Bigint> memory;
    std::string mask;
    std::vector<std::bitset<BITS>> vmasks;
    for (const auto& ins : vins) {
        if (ins.ins == Ins::mask) {     // work with Mask
            vmasks.clear();
            mask = std::get<Mask>(ins.data).mask;
            vmasks = get_all_masks(mask);
        } else {                        // work with Mem
            const auto& [index,value] = std::get<Mem>(ins.data);
            const auto bare = prep_addr(index, mask);

            for (const auto& m : vmasks) {
                auto addr = m | bare;
                memory[addr.to_ullong()] = value.to_ullong();
            }
        }
    }
    return memory_sum(memory);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    Timer t {};         // start of timing

    const auto input = read_input_lines();
    const auto program = parse_input(input);

    const auto part1 = decoder_v1(program);
    const auto part2 = decoder_v2(program);
    std::cout << std::setw(WIDTH) << "Part 1: " << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2: " << part2 << '\n';

    t.end(WIDTH);       // end of timing, print report
}
