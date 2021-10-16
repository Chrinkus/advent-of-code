#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void fill_map(istream& is, map<string, vector<string>>& m)
{
    while(is) {
        string line;
        getline(is, line);
        istringstream iss {line};

        string name, pipe;
        iss >> name >> pipe;

        if (is.eof() || pipe != "<->")
            break;

        vector<string> v;
        for (string s; iss >> s; ) {
            if (s.back() == ',')
                s.pop_back();
            v.push_back(s);
        }

        m[name] = v;
    }
}

void trace(map<string, vector<string>>& m, map<string, bool>& z,
        const string& id)
{
    for (auto& a : m[id])
        if (z[a] == false) {
            z[a] = true;       // toggle found to true
            trace(m, z, a);
        }
}

int map_trace(map<string, vector<string>>& m)
{
    map<string, bool> z;

    for (auto& a : m)           // set all to false
        z[a.first] = false;

    trace(m, z, "0");

    int count = 0;
    for (auto& a : z)
        if (a.second == true)
            ++count;

    return count;
}

int count_groups(map<string, vector<string>>& m)
{
    map<string, bool> z;

    for (auto& a : m)           // set all to false
        z[a.first] = false;

    int count = 0;
    for (auto& a : z)
        if (a.second == false) {
            trace(m, z, a.first);
            ++count;
        }

    return count;
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day12_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    map<string, vector<string>> programs;
    fill_map(ifs, programs);

    int with_0 = map_trace(programs);
    cout << "# grouped w/0: " << with_0 << '\n';

    int groups = count_groups(programs);
    cout << "# of groups: " << groups << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
