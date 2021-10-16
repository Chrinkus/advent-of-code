#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

bool is_valid(const string& phrase)
{
    istringstream iss {phrase};
    vector<string> words;

    for (string word; iss >> word; )
        words.push_back(word);

    int hits = 0;

    for (auto a : words)
        for (auto b : words)
            if (b == a) ++hits;

    return hits == words.size();        // each word should match one word in
                                        // the vector (itself)
}

int main()
try {
    string iname = "./day4_input.txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read file " + iname);

    int valid = 0;

    for (string line; getline(ifs, line); ) {

        if (is_valid(line)) ++valid;
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
