#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "Point.hpp"

using Point_steps = std::pair<Point,int>;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
template<typename InputIt1, typename InputIt2, typename OutputIt,
         typename Compare, typename Modify>
OutputIt transform_intersect(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2,
                             OutputIt d_first, Compare cmp, Modify mod)
{
    while (first1 != last1 && first2 != last2) {
        if (cmp(*first1, *first2)) {
            ++first1;
        } else {
            if (!cmp(*first2, *first1)) {
                *d_first++ = mod(*first1++, *first2);
            }
            ++first2;
        }
    }
    return d_first;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> vs;
    std::istringstream iss {s};
    for (std::string subs; std::getline(iss, subs, delim); )
        vs.push_back(subs);
    return vs;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
std::vector<Point_steps> plot_moves(const std::vector<std::string>& vm)
{
    std::vector<Point_steps> vp;
    Point p;
    int steps = 0;
    for (const auto& m : vm) {
        char dir = m[0];
        int num = std::stoi(m.substr(1));
        int dx, dy;
        switch (dir) {
            case 'U':   dx = 0; dy = 1;     break;
            case 'R':   dx = 1; dy = 0;     break;
            case 'D':   dx = 0; dy = -1;    break;
            case 'L':   dx = -1; dy = 0;    break;
        }
        for (auto i = 0; i < num; ++i) {
            p.move(dx, dy);
            vp.push_back(std::make_pair(p, ++steps));
        }
    }
    return vp;
}

Point_steps intersect_points(const Point_steps& ps1, const Point_steps& ps2)
{
    return std::make_pair(ps1.first, ps1.second + ps2.second);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
bool cmp_points(const Point_steps& a, const Point_steps& b)
{
    return a.first < b.first;
}

bool cmp_man_dist(const Point_steps& a, const Point_steps& b)
{
    return a.first.manhatten_distance() < b.first.manhatten_distance();
}

bool cmp_steps(const Point_steps& a, const Point_steps& b)
{
    return a.second < b.second;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    std::cout << "Advent of Code 2019 - Day 3 Alternate\n"
              << "Crossed Wires\n";

    // fetch and format input
    std::string input1;
    std::string input2;
    std::cin >> input1 >> input2;

    std::vector<std::string> vs1 = split(input1, ',');
    std::vector<std::string> vs2 = split(input2, ',');

	std::cout << "Input size: " << vs1.size() << '\n'
              << "  * first: " << vs1.front() << '\n'
              << "  * last : " << vs1.back() << '\n';

	std::cout << "Input size: " << vs2.size() << '\n'
              << "  * first: " << vs2.front() << '\n'
              << "  * last : " << vs2.back() << '\n';

    // run the wires
    std::vector<Point_steps> vpi1 = plot_moves(vs1);
    std::vector<Point_steps> vpi2 = plot_moves(vs2);

    // collect the intersecting points
    std::sort(std::begin(vpi1), std::end(vpi1), cmp_points);
    std::sort(std::begin(vpi2), std::end(vpi2), cmp_points);

    std::vector<Point_steps> vints;
    transform_intersect(std::begin(vpi1), std::end(vpi1),
                        std::begin(vpi2), std::end(vpi2),
                        std::back_inserter(vints), cmp_points,
                        intersect_points);

    auto closest_int = std::min_element(std::begin(vints), std::end(vints),
            cmp_man_dist)->first.manhatten_distance();

    auto soonest_int = std::min_element(std::begin(vints), std::end(vints),
            cmp_steps)->second;

	std::cout << "Part 1: " << closest_int << '\n';
	std::cout << "Part 2: " << soonest_int << '\n';
}
