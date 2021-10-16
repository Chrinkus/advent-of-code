#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

bool anagram_is_valid(const string& phrase)
    // handle anagrams by sorting each word's characters before inserting it
    // into the vector
{
    istringstream iss {phrase};
    vector<string> words;

    for (string word; iss >> word; ) {
        sort(word.begin(), word.end());
        words.push_back(word);
    }

    int hits = 0;

    for (auto a : words)
        for (auto b : words)
            if (a == b) ++hits;

    return hits == words.size();
}

int main()
try {
    string iname = "./day4_input.txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read file " + iname);

    int valid = 0;

    for (string line; getline(ifs, line); ) {

        if (anagram_is_valid(line)) ++valid;
    }

    cout << "The number of valid passphrases is: " << valid << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
