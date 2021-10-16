#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <bitset>

using namespace std;

vector<string> generate_inputs(const string& s)
{
    vector<string> v(128);

    for (int i = 0; i < v.size(); ++i) {
        ostringstream oss;
        oss << s << '-' << i;
        v[i] = oss.str();
    }

    return v;
}

vector<vector<int>> get_lengths(vector<string>& vs)
{
    vector<vector<int>> vv;
    for (auto& s : vs) {
        vector<int> vi;
        istringstream iss {s};
        for (char ch; iss >> ch; )
            vi.push_back(int(ch));
        vv.push_back(vi);
    }

    return vv;
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

vector<int> get_knot_hash(vector<int>& lengths)
{
    // create list
    vector<int> list(256);
    for (int i = 0; i < list.size(); ++i)
        list[i] = i;

    int pos = 0;
    int skip = 0;
    for (int i = 0; i < 64; ++i) {
        for (auto& a : lengths)
            knot_hash(list, a, pos, skip);
    }

    vector<int> hash;
    for (int i = 0; i < 16; ++i) {
        vector<int> dense;
        for (int j = 0; j < 16; ++j) {
            dense.push_back(list[i * 16 + j]);
        }

        int acc = 0;
        for (const auto& d : dense)
            acc ^= d;
        hash.push_back(acc);
    }

    return hash;
}

string get_hex_hash(vector<int>& v)
{
    ostringstream oss;
    for (auto& a : v) {
        if (a < 16)
            oss << hex << '0' << a;
        else
            oss << hex << a;
    }
    return oss.str();
}

string hex_to_bin(const string& s)
{
    ostringstream oss;

    for (auto a : s) {
        stringstream ss;
        ss << a;
        unsigned n;
        ss >> n;
        bitset<4> b(n);
        oss << b.to_string();
    }
    return oss.str();
}

string int_to_bin(vector<int>& vi)
{
    ostringstream oss;

    for (auto a : vi) {
        stringstream ss;
        ss << a;
        unsigned n;
        ss >> n;
        bitset<8> b(n);
        oss << b.to_string();
    }
    return oss.str();
}

int count_used(vector<string>& v)
{
    int count = 0;
    for (auto& s : v)
        for (auto& c : s)
            if (c == '1')
                ++count;
    return count;
}

void detect_flip(vector<string>& v, int i, int j)
{
    v[i][j] = '0';
    if (0 < j && v[i][j-1] == '1') detect_flip(v, i, j-1);
    if (j < v[i].size() && v[i][j+1] == '1') detect_flip(v, i, j+1);
    if (0 < i && v[i-1][j] == '1') detect_flip(v, i-1, j);
    if (i < v.size() && v[i+1][j] == '1') detect_flip(v, i+1, j);
}

int count_groups(vector<string> vs)
    // not pass by ref, need copy to flip bits
{
    int count = 0;
    for (int i = 0; i < vs.size(); ++i)
        for (int j = 0; j < vs[i].size(); ++j)
            if (vs[i][j] == '1') {
                ++count;
                detect_flip(vs, i, j);
            }
    return count;
}

int main()
try {
    string input = "hwlqcszp";
    //string input = "flqrgnkx";
    vector<string> strings = generate_inputs(input);

    vector<vector<int>> lengths = get_lengths(strings);

    vector<int> suffix { 17, 31, 73, 47, 23 };

    for (auto& a : lengths)
        for (auto& b : suffix)
            a.push_back(b);

    vector<vector<int>> hashes;
    for (auto& a : lengths)
        hashes.push_back(get_knot_hash(a));

    /* Removed: it is better to process the actual integer values into 8 bit
     * binary representations
     * vector<string> hex_hashes;
     * for (auto& a : hashes)
     *     hex_hashes.push_back(get_hex_hash(a));
     */

    vector<string> bin_str;
    for (auto& a : hashes)
        bin_str.push_back(int_to_bin(a));

    int used = count_used(bin_str);
    cout << "The number of used is: " << used << '\n';

    int groups = count_groups(bin_str);
    cout << "The number of groups is: " << groups << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
