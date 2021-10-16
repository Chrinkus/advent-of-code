#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

using namespace std;

int get_points(istream& is)
{
    int score = 0;          // score value of current group
    int total = 0;          // running score

    bool garbage = false;   // garbage toggle

    char ch = '0';

    while(is >> ch) {
        switch (ch) {
            case '!':
                is >> ch;                       // burn a char
                break;
            case '{':
                if (!garbage) ++score;          // increase score level
                break;
            case '}':
                if (!garbage && score > 0) {
                    total += score;     // close a group and add score
                    --score;
                }
                break;
            case '<':
                if (!garbage) garbage = true;
                break;
            case '>':
                if (garbage) garbage = false;
                break;
            default:
                // ignore chars
                break;
        }
    }
    return total;
}

int main()
try {
    // access to input/test files
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day9_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    // solution code
    int total = get_points(ifs);
    cout << "The total score is: " << total << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
