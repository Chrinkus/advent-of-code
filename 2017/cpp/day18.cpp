#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <sstream>

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
    if (op == "add") m[reg] += value(m);
    if (op == "mul") m[reg] *= value(m);
    if (op == "mod") m[reg] %= value(m);
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

long execute_loop(std::vector<Instruction>& v, std::map<char, long >& m)
{
    long last = 0;
    int i = 0;
    while (i < v.size()) {
        if (v[i].op == "rcv" && m[v[i].reg] != 0) {
            return last;
        }
        else if (v[i].op == "jgz" && m[v[i].reg] != 0) {
            i += v[i].value(m);
        }
        else if (v[i].op == "snd") {
            last = m[v[i].reg];
            ++i;
        }
        else {
            v[i].execute(m);
            ++i;
        }
    }

    return 0;
}

void get_instructions(const std::string& iname, std::vector<Instruction>& vi)
{
    std::ifstream ifs {iname};
    if (!ifs) throw std::runtime_error("Could not read from file: " + iname);

    for (Instruction ins; ifs >> ins; )
        vi.push_back(ins);
}

void fill_registers(const std::vector<Instruction>& v, std::map<char, long>& m)
{
    for (auto& a : v)
        m[a.reg] = 0;
    m['1'] = 1;
}

struct Program {
    // No default constructor
    Program(int i, std::vector<Instruction>& vi, Program* p);

    void connect(Program* p) { other = p; }
    void send(long val) { if (other != nullptr) other->store(val); }
    void store(long val);
    long recover();

    void execute(std::vector<Instruction>&);

    int id;
    Program* other;
    std::map<char, long> regs;
    std::list<long> q;
    int cur;
    bool waiting;
    bool done;
    char target;
    long sent;
};

Program::Program(int i, std::vector<Instruction>& vi, Program* p = nullptr)
    : id{i}, other{p}, cur{0}, waiting{false}, done{false}, target{0}, sent{0}
{
    fill_registers(vi, regs);
    regs['p'] = i;
}

void Program::store(long val)
{
    if (waiting) {
        regs[target] = val;
        waiting = false;
        std::cout << "Cancelled waiting\n";
        ++cur;
    }
    else {
        q.push_back(val);
    }
}

long Program::recover()
{
    long n = q.front();
    q.pop_front();
    return n; 
}

void Program::execute(std::vector<Instruction>& vi)
{
    if (!(waiting || done)) {
        Instruction ins = vi[cur];

        if (ins.op == "jgz") {
            if (regs[ins.reg] > 0)          // THE BUG (was compare to == 0)
                cur += ins.value(regs);
            else
                ++cur;
        }
        else if (ins.op == "snd") {
            send(regs[ins.reg]);
            ++sent;
            ++cur;
        }
        else if (ins.op == "rcv") {
            if (q.size() == 0) {
                waiting = true;
                target = ins.reg;
            }
            else {
                regs[ins.reg] = recover();
                ++cur;
            }
        }
        else {
            ins.execute(regs);
            ++cur;
        }
    }
}

void duel(Program* p0, Program* p1, std::vector<Instruction>& vi)
{
    while (!((p0->waiting || p0->done) && (p1->waiting || p1->done))) {
        std::cout << "Program0: " << p0->cur << ' '
                  << "Program1: " << p1->cur << ' ' << p1-> sent << '\n';
        p0->execute(vi);
        p1->execute(vi);
    }
}

int main(int argc, char* argv[])
try {
    std::string input = argc > 1 ? argv[1] : "";
    std::string filename = "./day18" + input + ".txt";

    std::vector<Instruction> vi;
    get_instructions(filename, vi);

    std::map<char, long > regs;
    fill_registers(vi, regs);

    long part_1 = execute_loop(vi, regs);

    Program zero {0, vi};
    Program one {1, vi, &zero};
    zero.connect(&one);

    duel(&zero, &one, vi);

    long part_2 = one.sent;

    std::cout << "The solution to part 1 is: " << part_1 << '\n';
    std::cout << "The solution to part 2 is: " << part_2 << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
