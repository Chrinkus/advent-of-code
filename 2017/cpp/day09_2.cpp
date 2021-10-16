#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

int get_garbage(std::istream& is)
{
    int score = 0;          // score value of current group
    int total = 0;          // running score
    int gbg = 0;            // garbage characters

    bool garbage = false;   // garbage toggle

    char ch = '0';

    while(is >> ch) {
        switch (ch) {
            case '!':
                is >> ch;               // burn a char
                break;
            case '{':
                if (garbage)
                    ++gbg;
                else
                    ++score;            // increase score level
                break;
            case '}':
                if (!garbage && score > 0) {
                    total += score;     // close a group and add score
                    --score;
                } else {
                    ++gbg;
                }
                break;
            case '<':
                if (garbage)
                    ++gbg;
                else
                    garbage = true;
                break;
            case '>':
                if (garbage)
                    garbage = false;
                break;
            default:
                if (garbage)
                    ++gbg;
                break;
        }
    }
    return gbg;
}

int main()
try {
    std::cout << "Which input shall be run?\n";
    std::string file;
    std::cin >> file;
    std::string iname = "../inputs/day9_" + file + ".txt";
    std::ifstream ifs {iname};
    if (!ifs) throw std::runtime_error("Could not read from file " + iname);

    int garbage = get_garbage(ifs);
    std::cout << "The total garbage removed is: " << garbage << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
