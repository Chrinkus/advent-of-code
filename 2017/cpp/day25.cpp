#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

struct Turing_mach {
    Turing_mach(vector<int> tt = vector<int>{0}, int cc = 0, char ss = 'A')
        : t{tt}, c{cc}, s{ss} { }

    int run_diagnostics() const;

    void resize();
    void step();

    vector<int> t;      // tape
    int c;              // cursor
    char s;             // state
};

int Turing_mach::run_diagnostics() const
{
    int count = 0;
    for (auto& a : t)
        if (a == 1)
            ++count;
    return count;
}

void Turing_mach::resize()
{
    if (c == 0) {
        t.insert(t.begin(), 0);
        ++c;
    }

    if (c == t.size()-1)
        t.push_back(0);
}

void Turing_mach::step()
{
    resize();

    switch(s) {
        case 'A':
            if (t[c] == 0) { t[c] = 1; ++c; s = 'B'; }
            else           { t[c] = 1; --c; s = 'E'; }
            break;
        case 'B':
            if (t[c] == 0) { t[c] = 1; ++c; s = 'C'; }
            else           { t[c] = 1; ++c; s = 'F'; }
            break;
        case 'C':
            if (t[c] == 0) { t[c] = 1; --c; s = 'D'; }
            else           { t[c] = 0; ++c; s = 'B'; }
            break;
        case 'D':
            if (t[c] == 0) { t[c] = 1; ++c; s = 'E'; }
            else           { t[c] = 0; --c; s = 'C'; }
            break;
        case 'E':
            if (t[c] == 0) { t[c] = 1; --c; s = 'A'; }
            else           { t[c] = 0; ++c; s = 'D'; }
            break;
        case 'F':
            if (t[c] == 0) { t[c] = 1; ++c; s = 'A'; }
            else           { t[c] = 1; ++c; s = 'C'; }
            break;
        default:
            throw runtime_error("unexpected state");
    }
}

int main()
try {
    int limit = 12459852;           // steps after which to perform checksum

    Turing_mach tur;

    for (int i = 0; i < limit; ++i)
        tur.step();

    int checksum = tur.run_diagnostics();

    cout << "The diagnostic checksum is: " << checksum << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
