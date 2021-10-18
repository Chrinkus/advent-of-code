#include <iostream>
#include <vector>
#include <sstream>
#include <array>
#include <algorithm>
#include <fstream>

class Op {
public:
    Op(int input);

    int code() const { return oc; }
    int mode(size_t i) const { return m[i]; }

private:
    int oc;
    std::array<int,3> m;
};

Op::Op(int input)
    : m{std::array<int,3>{0, 0, 0}}
{
    std::ostringstream oss;
    oss << input;
    std::string s {oss.str()};

    oc = s.size() == 1 ? std::stoi(s)
                       : std::stoi(s.substr(s.size() - 2));
    if (s.size() > 2) {
        std::string modes {s.substr(0, s.size() - 2)};
        std::reverse(std::begin(modes), std::end(modes));
        for (auto i = 0; i < modes.size(); ++i)
            m[i] = std::stoi(modes.substr(i, 1));
    }
}

std::ostream& operator<<(std::ostream& os, const Op& op)
{
    return os << op.code() << ": " << op.mode(0) << ','
              << op.mode(1) << ',' << op.mode(2);
}

int run_intcode(std::vector<int> vi, int input)
{
    for (size_t i = 0, p = 0; i < vi.size(); ) {
        Op op {vi[i]};
        switch (op.code()) {
            case 1:
                // add i+1 to i+2 store in i+3
                {
                    int& r1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& r2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    int& st = op.mode(2) == 0 ? vi[vi[i+3]] : vi[i+3];
                    st = r1 + r2;
                }
                i += 4;
                break;
            case 2:
                // mul i+1 to i+2 store in i+3
                {
                    int& r1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& r2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    int& st = op.mode(2) == 0 ? vi[vi[i+3]] : vi[i+3];
                    st = r1 * r2;
                }
                i += 4;
                break;
            case 3:
                // take int input and store in i+1
                std::cout << "seeking input..\n";
                //std::cin >> vi[vi[i+1]];
                vi[vi[i+1]] = input;
                i += 2;
                break;
            case 4:
                // output i+1
                std::cout << (op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1]) << '\n';
                i += 2;
                break;
            case 5:
                // jump if true
                {
                    int& p1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& p2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    if (p1)
                        i = p2;
                    else
                        i += 3;
                }
                break;
            case 6:
                // jump if false
                {
                    int& p1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& p2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    if (!p1)
                        i = p2;
                    else
                        i += 3;
                }
                break;
            case 7:
                // less than
                {
                    int& p1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& p2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    int& p3 = op.mode(2) == 0 ? vi[vi[i+3]] : vi[i+3];
                    p3 = p1 < p2;
                }
                i += 4;
                break;
            case 8:
                // equal
                {
                    int& p1 = op.mode(0) == 0 ? vi[vi[i+1]] : vi[i+1];
                    int& p2 = op.mode(1) == 0 ? vi[vi[i+2]] : vi[i+2];
                    int& p3 = op.mode(2) == 0 ? vi[vi[i+3]] : vi[i+3];
                    p3 = p1 == p2;
                }
                i += 4;
                break;
            case 99:
                // halt program
                return vi[0];
            default:
                std::cout << "Unknown op: " << op.code() << '\n';
                return -1;
        }
    }
    return -1;
}

int main(int argc, char** argv)
{
    std::cout << "Advent of Code 2019 - Day 5\n"
              << "Sunny with a Chance of Asteroids\n";

	std::vector<int> vi;
	for (int val; std::cin >> val; ) {
		vi.push_back(val);
        char ch;
        std::cin >> ch;
    }

	std::cout << "Input size: " << vi.size() << '\n'
              << "  * first: " << vi.front() << '\n'
              << "  * last : " << vi.back() << '\n';

	int part1 = run_intcode(vi, 1);
	int part2 = run_intcode(vi, 5);

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
