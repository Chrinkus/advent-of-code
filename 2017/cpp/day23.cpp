#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Instruction class & operators
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
struct Instruction {
    Instruction() : op{""}, reg{'0'}, val{""} { }
    Instruction(const std::string& o, char r, const std::string& v = "")
        : op{o}, reg{r}, val{v} { }

    long value(std::map<char, long>&);
    void execute(std::map<char, long>&);

    std::string op;
    char reg;
    std::string val;
};

long Instruction::value(std::map<char, long>& m)
{
    std::istringstream iss {val};

    long n;
    if (iss >> n)
        return n;

    iss.clear();
    char ch;
    iss >> ch;
    return m[ch];
}

void Instruction::execute(std::map<char, long>& m)
{
    if (op == "set") m[reg] = value(m);
    if (op == "sub") m[reg] -= value(m);
    if (op == "mul") m[reg] *= value(m);
}

std::istream& operator>>(std::istream& is, Instruction& ins)
{
    std::string o, v;
    char r;
    is >> o >> r;
    if (o != "rcv" && o != "snd")
        is >> v;

    Instruction ii {o, r, v};
    ins = ii;
    return is;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

vector<Instruction> load_instructions(const string& iname)
{
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file: " + iname);

    vector<Instruction> vi;
    for (Instruction ins; ifs >> ins; )
        vi.push_back(ins);

    return vi;
}

map<char, long> init_registers()
{
    map<char, long> mcl;
    for (char ch = 'a'; ch < 'i'; ++ch)
        mcl[ch] = 0;
    mcl['1'] = 1;

    return mcl;
}

long execute_loop(std::vector<Instruction>& v, std::map<char, long >& m)
{
    long mults = 0;
    int i = 0;
    while (i < v.size()) {
        if (v[i].op == "jnz" && m[v[i].reg] != 0) {
            i += v[i].value(m);
        }
        else {
            if (v[i].op == "mul") ++mults;

            v[i].execute(m);
            ++i;
        }
    }

    return mults;
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day23" + input + ".txt";

    vector<Instruction> vi = load_instructions(iname);
    map<char, long> registers = init_registers();

    long part_1 = execute_loop(vi, registers);

    cout << "The answer to part 1 is: " << part_1 << '\n';
    //cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
