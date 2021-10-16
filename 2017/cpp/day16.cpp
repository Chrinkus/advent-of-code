#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

std::vector<std::string> get_input(const std::string& fname)
{
    std::ifstream ifs {fname};
    if (!ifs) throw std::runtime_error("Could not read from file" + fname);

    std::vector<std::string> moves;
    while (ifs) {
        std::ostringstream oss;
        char ch;
        while (ifs >> ch && ch != ',') {
            oss << ch;
        }
        moves.push_back(oss.str());
    }

    return moves;
}

void apply_move(const std::string& move, std::string& s)
{
    const int l = s.size();
    std::istringstream iss {move};
    char ch;
    iss >> ch;

    switch (ch) {
        case 's':
            {
                int n;
                iss >> n;
                int pos = l - n;
                std::string spun = s.substr(pos, n) + s.substr(0, pos);
                s = spun;
            }
            break;
        case 'x':
            {
                int n1, n2;
                char ch;
                iss >> n1 >> ch >> n2;
                char t = s[n1];
                s[n1] = s[n2];
                s[n2] = t;
            }
            break;
        case 'p':
            {
                char ch1, ch2, ch3;
                iss >> ch1 >> ch2 >> ch3;
                int n1 = s.find(ch1);
                int n2 = s.find(ch3);
                s[n1] = ch3;
                s[n2] = ch1;
            }
            break;
        default:
            break;
    }
}

std::string dance(std::vector<std::string>& moves, std::string s)
    // 2nd arg is copy to change and return w/o changing original
{
    for (auto& m : moves)
        apply_move(m, s);

    return s;
}

std::string billion_dance(std::vector<std::string>& moves, std::string s)
    // Not DRY but alternative is a side-effect approach..
{
    std::string compare = s;

    int repeat = 0;
    do {
        for (auto& m : moves)
            apply_move(m, compare);
        ++repeat;
    } while (s != compare);

    int remain = 1000000000 % repeat;
    for (int i = 0; i < remain; ++i)
        for (auto& m : moves)
            apply_move(m, s);

    return s;
}

int main(int argc, char* argv[])
try {
    std::string file = argc > 1 ? argv[1] : "";

    std::vector<std::string> input = get_input("./day16" + file + ".txt");

    std::string dancers = "abcdefghijklmnop";

    std::string part_1 = dance(input, dancers);
    std::string part_2 = billion_dance(input, dancers);

    std::cout << "The solution to part 1 is: " << part_1 << '\n';
    std::cout << "The solution to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
