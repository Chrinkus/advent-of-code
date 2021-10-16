#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

struct Grid {
    Grid() : g{vector<string>()} { }
    
    explicit Grid(vector<string>& gg) : g{gg} { }
    explicit Grid(const string& s);

    int size() const { return g.size(); }
    string& operator[](int n) { return g[n]; }
    string operator[](int n) const { return g[n]; }

    vector<string> g;
};

Grid::Grid(const string& s)
    : g{vector<string>()}
{
    istringstream iss {s};
    while (iss) {
        string ss = "";
        for (char ch; iss >> ch && ch != '/'; )
            ss += ch;
        g.push_back(ss);
    }
}

Grid rotate(const Grid& g)
{
    vector<string> r;
    for (int j = g.size()-1; j >= 0; --j) {
        string s = "";
        for (int i = 0; i < g.size(); ++i) {
            s += g[i][j];
        }
        r.push_back(s);
    }
    Grid gg {r};
    return gg;
}

Grid flip(const Grid& g)
{
    vector<string> f;
    for (int i = 0; i < g.size(); ++i) {
        string s = "";
        for (int j = g[i].size()-1; j >= 0; --j) {
            s += g[i][j];
        }
        f.push_back(s);
    }
    Grid gg {f};
    return gg;
}

bool operator==(const Grid& a, const Grid& b)
{
    if (a.size() != b.size()) return false;

    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            return false;
    }

    return true;
}

ostream& operator<<(ostream& os, Grid& g)
{
    for (auto& a : g.g)
        os << a << '\n';
    return os;
}

struct Rule {
    Rule()
        : type{0}, input{ }, output{ } { }
    Rule(int t, Grid i, Grid o)
        : type{t}, input{i}, output{o} { }

    int type;                       // 2 for 2x2 or 3 for 3x3
    Grid input;
    Grid output;
};

istream& operator>>(istream& is, Rule& r)
{
    string ii, aa, oo;
    is >> ii >> aa >> oo;
    if (!is || aa != "=>") {
        return is;
    }

    int tt = ii.size() == 5 ? 2 : 3;
    Grid in {ii};
    Grid out {oo};

    Rule rr {tt, in, out};
    r = rr;

    return is;
}

void load_rules(const string& s, vector<Rule>& vr)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not read from file " + s);

    for (Rule r; ifs >> r; )
        vr.push_back(r);
}

vector<Grid> breakup(Grid& g)
{
    vector<Grid> broken;

    if (g.size() < 4) {
        broken.push_back(g);
        return broken;
    }

    int size = g.size() % 2 == 0 ? 2 : 3;

    for (int i = 0; i < g.size(); i += size) {
        for (int j = 0; j < g[i].size(); j += size) {
            vector<string> vs;
            for (int k = 0; k < size; ++k) {
                vs.push_back(g[i+k].substr(j, size));
            }
            broken.push_back(Grid{vs});
        }
    }

    return broken;
}

Grid check_flip(Grid&, vector<Rule>&);          // declaration

Grid check_rotation(Grid& g, vector<Rule>& rules)
{
    Grid gg = rotate(g);

    for (auto& a : rules)
        if (gg == a.input)
            return a.output;
    
    return check_flip(gg, rules);
}

Grid check_flip(Grid& g, vector<Rule>& rules)
{
    Grid gg = flip(g);
    for (auto& a : rules)
        if (gg == a.input)
            return a.output;

    return check_rotation(g, rules);        // original un-flipped Grid
}

Grid enhance(Grid& g, vector<Rule>& rules)
{
    for (auto& a : rules)
        if (g == a.input)
            return a.output;

    return check_rotation(g, rules);
}

Grid combine(vector<Grid>& vg)
{
    int size = sqrt(vg.size());

    vector<string> vs;
    for (int i = 0; i < vg.size(); i += size) {
        for (int j = 0; j < vg[i].size(); ++j) {
            string s = "";
            for (int k = 0; k < size; ++k) {
                s += vg[i + k][j];
            }
            vs.push_back(s);
        }
    }
    Grid combined {vs};
    return combined;
}

Grid enhance_img_to(vector<string>& img, vector<Rule>& r, int n)
{
    Grid gg {img};

    for (int i = 0; i < n; ++i) {
        
        vector<Grid> vg = breakup(gg);

        for (auto& g : vg) {
            g = enhance(g, r);
        }

        if (vg.size() > 1)
            gg = combine(vg);
        else
            gg = vg.front();
    }

    return gg;
}

int count_ons(Grid& g)
{
    int count = 0;
    for (int i = 0; i < g.size(); ++i)
        for (int j = 0; j < g[i].size(); ++j)
            if (g[i][j] == '#')
                ++count;

    return count;
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day21" + input + ".txt";

    vector<Rule> rules;
    load_rules(iname, rules);

    vector<string> start { ".#.", "..#", "###" };
    Grid enhanced = enhance_img_to(start, rules, 5);
    Grid enhanced_2 = enhance_img_to(start, rules, 18);

    int part_1 = count_ons(enhanced);
    int part_2 = count_ons(enhanced_2);

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
