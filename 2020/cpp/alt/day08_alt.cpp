#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

enum class Ins { acc, jmp, nop, err };

constexpr std::array<std::string_view,3> ins_names { "acc", "jmp", "nop" };

using Instructions = std::vector<std::pair<Ins,int>>;

Instructions parse_instructions(std::istream& is = std::cin)
{
    Instructions vii;
    std::string s;
    int qty;
    while (is >> s >> qty) {
        auto ins = get_instruction(s);
        vii.push_back(std::make_pair(ins, qty));
    }
    return vii;
}

Ins get_instruction(const std::string& s)
{
    auto it = std::find(std::begin(ins_names), std::end(ins_names), s);
    if (it == std::end(ins_names))
        return Ins::err;
    return static_cast<Ins>(std::distance(std::begin(ins_names), it));
}


class Game_boy {
public:

    Game_boy() = default;

    void load(const Instructions& input) { program = input; }
    std::pair<bool,int> run();

private:
    void execute(const std::pair<Ins,int>& ins);

    std::vector<std::pair<Ins,int>> program;
    int accumulator = 0;
    std::size_t index = 0;
    bool term_success = false;
};

std::pair<bool,int> Game_boy::run()
{
    std::vector<std::size_t> seen;
    while (index < program.size()) {
        auto it = std::find(std::begin(seen), std::end(seen), index);
        if (it != std::end(seen)) {
            return std::make_pair(term_success, accumulator);
        }
        seen.push_back(index);
        execute(program[index]);
    }
    term_success = true;
    return std::make_pair(term_success, accumulator);
}

void Game_boy::execute(const std::pair<Ins,int>& ins)
{
    const auto [ op, arg ] = ins;
    switch (op) {
        case Ins::acc:
            accumulator += arg;
            ++index;
            return;
        case Ins::jmp:
            index += arg;
            return;
        case Ins::nop:
            ++index;
            return;
        case Ins::err:
            std::cerr << "Error: unknown instruction\n";
            return;
    }
}