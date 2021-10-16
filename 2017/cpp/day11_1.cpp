#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

struct Tracker {
    Tracker() { }

    void calc_dist(std::map<std::string, int>&);

    int dist = 0;
    int max_d = 0;
};

void Tracker::calc_dist(std::map<std::string, int>& m)
{
    // check distance, update max_d if necessary
    std::vector<int> dd {
        std::abs(m["n"] - m["s"]),
        std::abs(m["ne"] - m["sw"]),
        std::abs(m["nw"] - m["se"])
    };

    std::sort(dd.begin(), dd.end());

    dist = dd[1] + dd[2];
    if (dist > max_d) max_d = dist;
}

void fill_directions(std::istream& is, std::vector<std::string>& v)
{
    while(is) {
        std::string d;

        char ch;
        while(is >> ch && isalpha(ch))
            d += ch;

        v.push_back(d);
    }
}

int main()
try {
    // get input
    std::cout << "Which input shall be run?\n";
    std::string file;
    std::cin >> file;
    std::string iname = "../inputs/day11_" + file + ".txt";
    std::ifstream ifs {iname};
    if (!ifs) throw std::runtime_error("Could not read from file " + iname);

    std::vector<std::string> directions;
    fill_directions(ifs, directions);

    // layout map
    std::map<std::string, int> dir_steps;
    dir_steps["n"] = 0;
    dir_steps["ne"] = 0;
    dir_steps["nw"] = 0;
    dir_steps["s"] = 0;
    dir_steps["se"] = 0;
    dir_steps["sw"] = 0;

    // track steps
    Tracker t;
    for (const auto& s : directions) {
        dir_steps[s]++;
        t.calc_dist(dir_steps);
    }

    std::cout << "Current steps: " << t.dist << '\t'
              << "Max steps: " << t.max_d << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
