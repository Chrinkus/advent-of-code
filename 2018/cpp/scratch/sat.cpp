#include <iostream>
#include <summed_area_table.hpp>

int main()
{
    auto sat = utils::Summed_area_table<int>(300, 300);
    std::cout << sat.size() << '\n';
}
