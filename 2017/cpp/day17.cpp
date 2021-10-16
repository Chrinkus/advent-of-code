#include <iostream>
#include <stdexcept>
#include <list>
#include <sstream>

// This is a mess, refactor at earliest convenience
// There is a shortcut solution. I've read it but don't understand it yet.

void fill_list(const int input, std::list<int>& sl)
{
    std::list<int>::iterator it = sl.begin();

    for (int i = 1; i < 2000; ++i) {
        for (int j = 0; j < input; ++j) {
            ++it;
            if (it == sl.end())
                it = sl.begin();
        }
        sl.insert(++it, i);
        --it;
    }
}

int find_next(std::list<int>& li, int val)
{
    bool found = false;
    for (auto& a : li) {
        if (found) return a;
        if (a == val) found = true;
    }
    return 0;
}

int main(int argc, char* argv[])
try {
    std::string str_input = argc > 1 ? argv[1] : "324";
    std::istringstream ss {str_input};
    int input;
    ss >> input;

    std::list<int> spinlock {0};

    fill_list(input, spinlock);

    bool found = false;
    for (auto& a : spinlock) {
        if (found) {
            std::cout << a << '\n';
            break;
        }
        if (a == 0) found = true;
    }
    int part_1 = find_next(spinlock, 2017);

    //std::cout << "The solution to part 1 is: " << part_1 << '\n';
    //std::cout << "The solution to part 2 is: " << part_2 << '\n';
}
catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    std::cerr << "Unknown exception\n";
    return 2;
}
