#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

void fill_from_file(std::istream& is, std::vector<int>& v)
    // get input data
{
    for (char ch; is >> ch; )
        v.push_back(int(ch));
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

    pos = (pos + length + skip) % list.size();
    ++skip;
}

void hex_out(int val)
{
    if (val < 16)
        std::cout << '0' << std::hex << val;
    else
        std::cout << std::hex << val;
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

    std::vector<int> suffix {17, 31, 73, 47, 23};

    for (const auto a : suffix)
        lengths.push_back(a);

    std::vector<int> list(256);
    for (int i = 0; i < list.size(); ++i)
        list[i] = i;

    int pos = 0;
    int skip = 0;
    for (int i = 0; i < 64; ++i) {
        for (const auto& a : lengths)
            knot_hash(list, a, pos, skip);
    }

    for (int i = 0; i < 16; ++i) {
        std::vector<int> dense;
        for (int j = 0; j < 16; ++j) {
            dense.push_back(list[i * 16 + j]);
        }

        int acc = 0;
        for (const auto& d : dense)
            acc ^= d;
        hex_out(acc);
    }
    std::cout << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
