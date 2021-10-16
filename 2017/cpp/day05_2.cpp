#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int get_steps_twist(istream& is)
{
    vector<int> v;                      // create vector and fill it
    for (int val; is >> val; )
        v.push_back(val);

    int steps = 0;                      // step counter
    for (int i = 0; i < v.size(); ) {
        int t = i;                      // stash i
        i += v[i];                      // jump!
        v[t] += v[t] < 3 ? 1 : -1;      // adjust jump value
        ++steps;
    }

    return steps;
}

int main()
try {
    cout << "Enter an input source:\n";
    string source;
    cin >> source;

    string iname = "../inputs/day5_" + source + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read file " + iname);

    int steps = get_steps_twist(ifs);
    cout << "The number of steps to reach exit is " << steps << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
