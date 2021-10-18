#include "Intcode_vm.hpp"

#include <sstream>
#include <algorithm>

// Ops are defined by having an op code and associated modes for the op
// parameters. The op code is the right-most two digits and the parameter modes
// read right to left after the two digits reserved for the op code.
//
// Parsing steps:
// - convert to string
// - extract op code
// - if more than 2 digits, extract modes starting from right to left.
//
// Concerns:
// - modes default to 0
// - single digit op codes have a padded zero if modes are explicitly stated
//
// Example:
// - 12008 - code 8, mode 0, 2, 1
Op::Op(int64_t input)
    : oc{0}, m{std::array<int,3>{0, 0, 0}}
{
    std::ostringstream oss;
    oss << input;
    std::string op {oss.str()};

    oc = op.size() == 1 ? std::stoi(op)
                        : std::stoi(op.substr(op.size() - 2));
    if (op.size() > 2) {
        std::string modes {op.substr(0, op.size() - 2)};
        std::reverse(std::begin(modes), std::end(modes));
        for (auto i = 0; i < modes.size(); ++i)
            m[i] = std::stoi(modes.substr(i, 1));
    }
}

// for debugging purposes
std::ostream& operator<<(std::ostream& os, const Op& op)
{
    return os << op.code() << ": " << op.mode(0) << ','
              << op.mode(1) << ',' << op.mode(2);
}

Intcode_vm::Intcode_vm(const std::string& program)
{
	std::istringstream iss {program};
	for (std::string sub; std::getline(iss, sub, ','); )        // split-ish
		vi.push_back(std::stoll(sub));
    vi.resize(vi.size() * 4, 0);                    // double size of "memory"
}

int64_t& Intcode_vm::get_param(int mode, size_t param)
{
    switch (mode) {
        case 0:     return vi[vi[param]];
        case 1:     return vi[param];
        case 2:     return vi[rb + vi[param]];
        default:    throw Bad_mode{};           // satisfies compiler warning
    }
}

void Intcode_vm::run(std::istream& is, std::ostream& os)
{
    if (halted())
        return;

    for (size_t i = ip; i < size(); ) {
        ip = i;
        Op op {vi[i]};
        switch (op.code()) {
            case 1: // add p1 to p2, store in p3
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    int64_t& p3 = get_param(op.mode(2), i + 3);
                    p3 = p1 + p2;
                }
                i += 4;
                break;
            case 2: // mul p1 to p2, store in p3
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    int64_t& p3 = get_param(op.mode(2), i + 3);
                    p3 = p1 * p2;
                }
                i += 4;
                break;
            case 3: // input int64_t, store in p1
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    if (!(is >> p1)) {
                        is.clear();
                        return;
                    }
                }
                i += 2;
                break;
            case 4: // output value in p1
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    os << p1 << '\n';
                }
                i += 2;
                break;
            case 5: // jump if p1 true, to p2
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    i = p1 != 0 ? p2 : i + 3;
                }
                break;
            case 6: // jump if p1 false, to p2
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    i = p1 == 0 ? p2 : i + 3;
                }
                break;
            case 7: // less than, p1 < p2, store in p3
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    int64_t& p3 = get_param(op.mode(2), i + 3);
                    p3 = p1 < p2;
                }
                i += 4;
                break;
            case 8: // equal, p1 == p2, store in p3
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    int64_t& p2 = get_param(op.mode(1), i + 2);
                    int64_t& p3 = get_param(op.mode(2), i + 3);
                    p3 = p1 == p2;
                }
                i += 4;
                break;
            case 9: // adjust relative base by p1
                {
                    int64_t& p1 = get_param(op.mode(0), i + 1);
                    rb += p1;
                }
                i += 2;
                break;
            case 99: // halt
                h = true;
                return;
        }
    }
}

void Intcode_vm::config(int64_t noun, int64_t verb)
{
    vi[1] = noun;
    vi[2] = verb;
}
