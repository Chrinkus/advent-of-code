#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

struct Layer {
    Layer() { };
    Layer(int d, int r)
        : depth{d}, range{r} { }

    void move_scanner();
    int severity() const { return depth * range; }

    int depth = 0;
    int range = 0;
    int scanner = 0;
    bool ascending = true;
};

void Layer::move_scanner()
{
    if (ascending && scanner < range) {
        ++scanner;
        if (scanner == range - 1)
            ascending = false;
    } else {
        --scanner;
        if (scanner == 0)
            ascending = true;
    }
}

istream& operator>>(istream& is, Layer& l)
{
    int d, r;
    char ch;
    is >> d >> ch >> r;
    Layer ll {d, r};
    l = ll;
    return is;
}

ostream& operator<<(ostream& os, Layer& l)
{
    return os << l.depth << '\t' << l.range << '\t'
              << l.scanner << '\t' << l.ascending;
}

void fill_from_file(istream& is, vector<Layer>& v)
{
    for (Layer l; is >> l; )
        v.push_back(l);
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day13_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    vector<Layer> firewall;
    fill_from_file(ifs, firewall);

    int max_depth = firewall.back().depth;

    int n = 0;                  // current location in firewall
    int total_sev = 0;
    for (int i = 0; i <= max_depth; ++i) {
        //cout << "PS " << i << " " << firewall[n] << '\n';
        if (i == firewall[n].depth) {
            if (firewall[n].scanner == 0)
                total_sev += firewall[n].severity();

            ++n;                // the next firewall layer
        }

        cout << firewall.back().scanner << '\t';
        for (auto& f : firewall)
            f.move_scanner();
    }
    cout << '\n';

    cout << "The total severity was: " << total_sev << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
