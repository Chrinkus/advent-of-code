#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

vector<int>& fill_from_file(istream& is, vector<int>& v)
{
    for (int val; is >> val; )
        v.push_back(val);

    return v;
}

int get_cycles(vector<int>& v)
{
    int cycles = 0;
    vector<string> snaps;

    while(true) {
        // convert v to string and store snapshot
        ostringstream snap;
        for (auto a : v)
            snap << a << " ";

        cout << snap.str() << '\n';     // shows progress while running

        for (auto a : snaps)
            if (a == snap.str()) {
                return cycles;
            }

        snaps.push_back(snap.str());

        // find max and its location
        int loc = 0;
        int max = v[loc];
        for (int i = 0; i < v.size(); ++i)
            if (v[i] > max) {
                max = v[i];
                loc = i;
            }

        // remove max and redistribute it starting at loc + 1
        v[loc] = 0;
        for (int i = loc + 1; max > 0; --max) {
            i = i % v.size();
            v[i] += 1;
            ++i;
        }
        ++cycles;               // redistribution complete
    }

    return 0;
}

int main()
try {
    cout << "Which input shall be tested?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day6_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Cannot read from file " + iname);

    vector<int> vi;
    fill_from_file(ifs, vi);

    int cycles = get_cycles(vi);

    cout << "The number of cycles completed was: " << cycles << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
