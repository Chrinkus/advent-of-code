#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Packet {
    explicit Packet(const vector<char>&);

    char move(vector<vector<char>>&);
    void turn(vector<vector<char>>&);
    void inc_steps() { ++steps; }

    int x;
    int y;
    char dir;
    string letters;
    int steps;
};

Packet::Packet(const vector<char>& v)
    : y{0}, dir{'d'}, letters{""}, steps{0}
{
    for (int i = 0; i < v.size(); ++i)
        if (!isspace(v[i]))
            x = i;
}

char Packet::move(vector<vector<char>>& vv)
{
    switch(dir) {
        case 'u': --y; break;
        case 'd': ++y; break;
        case 'l': --x; break;
        case 'r': ++x; break;
        default:
            throw runtime_error("move(): unknown direction");
    }
    return vv[y][x];
}

void Packet::turn(vector <vector<char>>& vv)
{
    if (dir == 'u' || dir == 'd')
        dir = isspace(vv[y][x-1]) ? 'r' : 'l';
    else if (dir == 'l' || dir == 'r')
        dir = isspace(vv[y-1][x]) ? 'd' : 'u';
}

vector<vector<char>> load_diagram(const string& s)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not open file " + s);

    vector<vector<char>> vvc;
    for (string line; getline(ifs, line) && line.size() > 0; ) {
        vector<char> vc;
        for (auto& ch : line)
            vc.push_back(ch);
        vvc.push_back(vc);
    }

    return vvc;
}

void traverse(Packet& p, vector<vector<char>>& vv)
{
    while (true) {
        char next = p.move(vv);
        p.inc_steps();

        if (isspace(next)) {            // end! return result
            return;
        }
        else if (isalpha(next)) {       // found letter! save it and keep on
            p.letters += next;
        }
        else if (next == '+') {         // only change direction if '+'
            p.turn(vv);
        }
    }
    
    throw runtime_error("Somehow false..");
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day19" + input + ".txt";

    vector<vector<char>> diagram = load_diagram(iname);

    Packet pac {diagram[0]};
    traverse(pac, diagram);

    string part_1 = pac.letters;
    int part_2 = pac.steps;

    cout << "The answer to part 1 is: " << part_1 << '\n';
    cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
