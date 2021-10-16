#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Component class
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
struct Component {
    Component(int aa = 0, int bb = 0, bool uu = false)
        : a{aa}, b{bb}, used{uu} { }

    int a;
    int b;
    bool used;
};

istream& operator>>(istream& is, Component& c)
{
    int n1, n2;
    char ch;
    is >> n1 >> ch >> n2;

    Component cc {n1, n2};
    c = cc;
    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Read-in input
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
vector<Component> load_components(const string& s)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not read from file " + s);

    vector<Component> vc;
    for (Component c; ifs >> c; )
        vc.push_back(c);

    return vc;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// The recursive solution
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void build (vector<Component> vc, vector<int>& bridges, vector<int>& lengths,
        int strength = 0, int connect = 0, int len = 0)
    // deliberately passing copy of vc
{
    for (auto& a : vc)
        if (!a.used)
            if (a.a == connect || a.b == connect) {
                a.used = true;
                int new_strength = strength + a.a + a.b;
                int new_connect = a.a == connect ? a.b : a.a;
                int new_len = len + 1;
                build(vc, bridges, lengths, new_strength, new_connect,
                        new_len);
                a.used = false;         // tha bug
            }

    bridges.push_back(strength);
    lengths.push_back(len);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Max finder
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int get_max(vector<int>& v)
{
    int max = 0;

    for (auto& a : v)
        if (a > max)
            max = a;

    return max;
}

int get_strongest_longest(vector<int>& b, vector<int>& l)
{
    int longest = get_max(l);

    vector<int> li;
    for (int i = 0; i < l.size(); ++i)
        if (l[i] == longest)
            li.push_back(i);

    int longest_strongest = 0;
    for (auto& a : li)
        if (b[a] > longest_strongest)
            longest_strongest = b[a];

    return longest_strongest;
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day24" + input + ".txt";

    vector<Component> comps = load_components(iname);

    vector<int> bridges;
    vector<int> lengths;
    
    build(comps, bridges, lengths);

    int part_1 = get_max(bridges);
    int part_2 = get_strongest_longest(bridges, lengths);

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
