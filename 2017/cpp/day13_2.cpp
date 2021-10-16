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
    void reset() { scanner = 0; ascending = true; }
    void fast_forward(int);

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

void Layer::fast_forward(int n)
{
    int max = range - 1;
    int mod = n % (2 * range - 2);
    scanner = mod > max ? max - (mod - max) : mod;
    ascending = mod > max - 1 ? false : true;
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

    bool caught = true;
    int delay = 0;
    while (caught) {
        // Consume delay
        for (auto& f : firewall)
            f.fast_forward(delay);

        caught = false;
        int n = 0;                  // current location in firewall
        for (int i = 0; i <= max_depth; ++i) {
            if (i == firewall[n].depth) {
                if (firewall[n].scanner == 0) {
                    caught = true;
                    break;
                }

                ++n;                // the next firewall layer
            }

            for (auto& f : firewall)
                f.move_scanner();
        }
        if (caught) ++delay;

        for (auto& f : firewall)
            f.reset();
    }

    cout << "The required delay to not get caught is: " << delay << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
