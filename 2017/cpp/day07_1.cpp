#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Tower {
    Tower(const string& n, int w, vector<string>& c)
        : name{n}, weight{w}, child_names{c} { }

    Tower(const string& n, int w)
        : name{n}, weight{w} { }

    Tower()
        : name{""}, weight{0}, child_names{vector<string>()} { }

    string name;
    int weight;
    vector<string> child_names;
};

istream& operator>>(istream& is, Tower& tt)
{
    string line;
    getline(is, line);
    istringstream iss {line};

    string nn;
    int ww;
    char ch1, ch2;
    vector<string> v;

    iss >> nn >> ch1 >> ww >> ch2;
    if (ch1 != '(' || ch2 != ')') {
        is.clear(ios_base::failbit);
        return is;
    }

    string a;
    if (iss >> a && a == "->") {
        for (string s; iss >> s; ) {
            if (s.back() == ',') s.pop_back();
            v.push_back(s);
        }
        tt = Tower{nn, ww, v};
    } else {
        tt = Tower{nn, ww};
    }

    return is;
}

ostream& operator<<(ostream& os, Tower& tt)
{
    return os << tt.name << '\t'
              << tt.weight << '\t'
              << tt.child_names.size() << '\n';
}

void fill_from_file(istream& is, vector<Tower>& v)
{
    for (Tower t; is >> t; )
        v.push_back(t);
}

Tower* filter_out_children(const vector<Tower*>& pv) {
    vector<Tower*> vt;

    for (const auto& a : pv) {
        bool match = false;

        for (const auto& b : a->child_names)
            for (const auto& c : pv)
                if (c->name == b)
                    match = true;

        if (match) vt.push_back(a);
    }
    if (vt.size() > 1)
        return filter_out_children(vt);
    else
        return vt[0];
}

Tower* get_root(vector<Tower>& v)
{
    vector<Tower*> pvt;
    for (auto& a : v)
        pvt.push_back(&a);

    return filter_out_children(pvt);
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day7_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    vector<Tower> vt;
    fill_from_file(ifs, vt);

    Tower* pr = get_root(vt);

    cout << pr->name << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
