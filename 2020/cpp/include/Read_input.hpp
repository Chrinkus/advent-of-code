#pragma once

#include <iostream>     // needed for reading
#include <string>       // some platforms need this
#include <vector>       // file sizes are usually unknown
#include <algorithm>    // for sorting

template<typename Data>
auto read_input(std::istream& is = std::cin)
{
    std::vector<Data> vd;
    for (Data d; is >> d; ) {
        vd.push_back(d);
    }
    return vd;
}

// No template param, usually always std::string
auto read_input_lines(std::istream& is = std::cin)
{
    std::vector<std::string> vs;
    for (std::string s; std::getline(is, s); ) {
        vs.push_back(s);
    }
    return vs;
}

template<typename Data>
auto read_input_and_sort(std::istream& is = std::cin)
{
    auto vd = read_input<Data>(is);
    std::sort(std::begin(vd), std::end(vd));
    return vd;
}

/*
template<typename Data>
auto read_data_and_sort_each(std::istream& is = std::cin)
{
    auto vd = read_data<Data>(is);
    for (auto& d : vd) {
        std::sort(std::begin(d), std::end(d));
    }
}
*/