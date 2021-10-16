#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

// Satisfies both problems

void fill_from_file(istream& is, vector<string>& v)
{
    for (string line; getline(is, line); )
        v.push_back(line);
}

void assign_registers(vector<string>& inst, map<string, int>& u)
{
    for (string s : inst) {
        istringstream iss {s};
        string reg;
        iss >> reg;
        u[reg] = 0;
    }
}

struct Instruction {
    Instruction(const string& s);

    bool test(map<string, int>& u);
    void execute(map<string, int>& u);

    string reg;
    char op;
    int val;
    // condition
    string r;
    string c;
    int v;
};

Instruction::Instruction(const string& s)
{
    istringstream iss {s};

    string o;
    iss >> reg >> o >> val;
    op = o == "dec" ? '-' : '+';

    string _;
    iss >> _ >> r >> c >> v;
}

bool Instruction::test(map<string, int>& u) 
{
    if (c == ">")  return u[r] > v;
    if (c == "<")  return u[r] < v;
    if (c == ">=") return u[r] >= v;
    if (c == "<=") return u[r] <= v;
    if (c == "==") return u[r] == v;
    if (c == "!=") return u[r] != v;

    throw runtime_error("unrecognized op");
}

void Instruction::execute(map<string, int>& u)
{
    // test condition
    if (test(u))
        op == '+' ? u[reg] += val : u[reg] -= val;
}

int find_largest(map<string, int>& u)
{
    int largest = -100000;
    for (const auto& a : u)
        if (a.second > largest)
            largest = a.second;
    return largest;
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day8_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    vector<string> instructions;
    fill_from_file(ifs, instructions);

    map<string, int> registers;
    assign_registers(instructions, registers);

    int largest_ever = 0;
    for (const auto& a : instructions) {
        Instruction istr {a};
        istr.execute(registers);

        // Part 2
        int current_largest = find_largest(registers);
        if (current_largest > largest_ever)
            largest_ever = current_largest;
    }

    for (const auto& a : registers)
        cout << a.first << '\t' << a.second << '\n';

    cout << "Largest: " << find_largest(registers) << '\n';
    cout << "Largest ever: " << largest_ever << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
