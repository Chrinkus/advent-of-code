#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void fill_from_file(std::istream& is, std::vector<int>& v)
    // get input data
{
    while (is) {
        int val;
        is >> val;
        v.push_back(val);

        char ch;
        is >> ch;           // throw away comma
    }
}

void reverse(std::vector<int>& v)
{
    int b = 0;
    int e = v.size() - 1;
    while (b < e) {
        int t = v[b];
        v[b] = v[e];
        v[e] = t;
        ++b;
        --e;
    }
}

void knot_hash(std::vector<int>& list, const int length, int& pos, int& skip)
    // grab a sublist
    // reverse sublist
    // replace sublist section in list with sublist
    // move pos by length + skip
    // increase skip
{
    std::vector<int> sublist;
    for (int i = pos; i < pos + length; ++i) {
        int ii = i % list.size();
        sublist.push_back(list[ii]);
    }

    reverse(sublist);

    int s = 0;
    int i = pos;
    while (i < pos + length) {
        int ii = i % list.size();
        list[ii] = sublist[s];
        ++i;
        ++s;
    }

    pos += length + skip;
    ++skip;
}

int main()
try {
    std::cout << "Which input shall be run?\n";
    std::string file;
    std::cin >> file;
    std::string iname = "../inputs/day10_" + file + ".txt";
    std::ifstream ifs {iname};
    if (!ifs) throw std::runtime_error("Could not read from file " + iname);

    std::vector<int> lengths;
    fill_from_file(ifs, lengths);

    std::vector<int> list(256);
    for (int i = 0; i < list.size(); ++i)
        list[i] = i;

    int pos = 0;
    int skip = 0;
    for (const auto& a : lengths)
        knot_hash(list, a, pos, skip);

    std::cout << "The product of first two list values "
         << list[0] << " and " << list[1] << " is: "
         << list[0] * list[1] << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
