#include <iostream>
#include <list>
#include <algorithm>

#include "circular_list.hpp"

int main()
{
    std::cout << "Circular_list test\n";

    std::list<int> li;
    auto lit = li.begin();
    li.insert(lit, 42);

    for (auto v : li)
        std::cout << v << '\n';

    /*
    Circular_list<int> clist;
    auto it = clist.begin();
    clist.insert(it, 37);

    for (auto v : clist)
        std::cout << v << '\n';
    */
}
