#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int get_steps(istream& is)
{
    vector<int> v;

    for (int val; is >> val; )
        v.push_back(val);

    int steps = 0;
    for (int i = 0; i < v.size(); ) {
        i += v[i]++;
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

    int steps = get_steps(ifs);
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
