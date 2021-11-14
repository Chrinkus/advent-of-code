#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

#include "Read_input.hpp"       // simple, handy reader
#include "Timer.hpp"            // for timing solution
#include "Matrix11.hpp"         // Stroustrup
#include "MatrixIO11.hpp"       // Stroustrup
constexpr int WIDTH = 6;        // for output alignment

using namespace Numeric_lib;    // Matrix
using Index = std::size_t;

/* 2D tools from day 11
constexpr std::array<std::pair<int,int>,8> dirs_2d { {
    {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1}
} };

auto count_adjacents(const Matrix<char,2>& mx, const Index& x, const Index& y)
{
    auto count = 0;
    for (const auto& [dx,dy] : dirs_2d) {
        Index xx = x + dx, yy = y + dy;
        if (0 <= xx && xx < mx.dim2() && 0 < yy && yy < mx.dim1() &&
                mx(yy,xx) == '#')
            ++count;
    }
    return count;
}
*/
constexpr std::array<std::tuple<int,int,int>,26> dirs_3d { {
    {-1,-1,-1}, {0,-1,-1},  {1,-1,-1},  //
    {-1,0,-1},  {0,0,-1},   {1,0,-1},   // front layer
    {-1,1,-1},  {0,1,-1},   {1,1,-1},   // 

    {-1,-1,0},  {0,-1,0},   {1,-1,0},   //
    {-1,0,0},   /*0,0,0*/   {1,0,0},    // middle layer
    {-1,1,0},   {0,1,0},    {1,1,0},    //

    {-1,-1,1},  {0,-1,1},   {1,-1,1},   //
    {-1,0,1},   {0,0,1},    {1,0,1},    // back layer
    {-1,1,1},   {0,1,1},    {1,1,1}     //
} };

const auto dirs_4d = [] {
        constexpr Index SIZE_4D = 80;
        std::array<std::tuple<int,int,int,int>,SIZE_4D> arr;
        Index i = 0;
        for (int w = -1; w < 2; ++w)
            for (int z = -1; z < 2; ++z)
                for (int y = -1; y < 2; ++y)
                    for (int x = -1; x < 2; ++x) {
                        if (w == 0 && z == 0 && y == 0 && x == 0)
                            continue;
                        arr[i++] = std::make_tuple(w,z,y,x);
                    }
        return arr;
    }();
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 1 - Stepping up to 3D
auto parse_2d_input(const std::vector<std::string>& input)
{
    Matrix<char,2> mx (input.front().size(), input.size());
    for (::Index i = 0; i < input.size(); ++i) {
        for (::Index j = 0; j < input.front().size(); ++j) {
            mx(i,j) = input[i][j];
        }
    }
    return mx;
}

auto convert_to_3d(const Matrix<char,2>& mx2, const char fill)
    // assumes square to cube
{
    Matrix<char,3> mx3 (mx2.dim1(), mx2.dim2(), mx2.dim1());
    mx3 = fill;
    for (Index i = 0, k = mx3.dim1() / 2; i < mx2.dim1(); ++i) {
        for (Index j = 0; j < mx2.dim2(); ++j) {
            mx3(k,i,j) = mx2(i,j);
        }
    }
    return mx3;
}

auto grow_cube(const Matrix<char,3>& mx_old, const char fill, Index delta = 0)
{
    if (delta == 0) delta = mx_old.dim1() / 2;  // default growth is by half
    Matrix<char,3> mx_new (mx_old.dim1() + delta,
                           mx_old.dim2() + delta,
                           mx_old.dim3() + delta);
    mx_new = fill;
    for (Index i = 0, pad = delta / 2; i < mx_old.dim1(); ++i) {
        for (Index j = 0; j < mx_old.dim2(); ++j) {
            for (Index k = 0; k < mx_old.dim3(); ++k) {
                mx_new(i+pad,j+pad,k+pad) = mx_old(i,j,k);
            }
        }
    }
    return mx_new;
}

auto count_adjacents(const Matrix<char,3>& mx, const Index& x, const Index& y,
        const Index & z)
{
    auto count = 0;
    for (const auto& [dx, dy, dz] : dirs_3d) {
        Index xx = x + dx, yy = y + dy, zz = z + dz;
        if (0 <= xx && xx < mx.dim2() &&
            0 <= yy && yy < mx.dim1() &&
            0 <= zz && zz < mx.dim3()) {
            if (mx(yy,xx,zz) == '#')
                ++count;
        }
    }
    return count;
}

auto advance_cycle(const Matrix<char,3>& mx)
{
    auto mnext = mx;        // should do copy
    for (Index y = 0; y < mx.dim1(); ++y) {
        for (Index x = 0; x < mx.dim2(); ++x) {
            for (Index z = 0; z < mx.dim3(); ++z) {
                const auto ch = mx(y,x,z);
                switch (ch) {
                case '.':
                    if (count_adjacents(mx, x, y, z) == 3) {
                        mnext(y,x,z) = '#';
                    }
                    break;
                case '#':
                    {
                        auto res = count_adjacents(mx, x, y, z);
                        if (res < 2 || 3 < res) {
                            mnext(y,x,z) = '.';
                        }
                        break;
                    }
                }
            }
        }
    }
    return mnext;
}

auto count_active(const Matrix<char,3>& mx)
{
    return std::count(mx.data(), mx.data() + mx.size(), '#');
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Part 2 - The 4th Dimension!!
auto convert_to_4d(const Matrix<char,3>& mx3, const char fill)
    // assumes cube to tesseract
{
    Matrix<char,4> mx4 (mx3.dim1(), mx3.dim1(), mx3.dim2(), mx3.dim3());
    mx4 = fill;
    const Index w = mx4.dim1() / 2;     // put cube in "middle" of tesseract
    for (Index z = 0; z < mx3.dim1(); ++z) 
        for (Index y = 0; y < mx3.dim2(); ++y)
            for (Index x = 0; x < mx3.dim3(); ++x)
                mx4(w,z,y,x) = mx3(z,y,x);
    return mx4;
}
/* Not needed, not tested
auto grow_tesseract(const Matrix<char,4>& mold, const char fill, Index delta)
{
    Matrix<char,4> mnew (mold.dim1() + delta,
                         mold.dim2() + delta,
                         mold.dim3() + delta,
                         mold.dim4() + delta);
    mnew = fill;
    for (Index w = 0, pad = delta / 2; w < mold.dim1(); ++w)
        for (Index z = 0; z < mold.dim2(); ++z)
            for (Index y = 0; y < mold.dim3(); ++y)
                for (Index x = 0; x < mold.dim4(); ++x)
                    mnew(w+pad,z+pad,y+pad,x+pad) = mold(w,z,y,x);
    return mnew;
}
*/
auto count_adjacents(const Matrix<char,4>& mx, const Index& w, const Index& z,
        const Index& y, const Index& x)
{
    auto count = 0;
    for (const auto& [dw,dz,dy,dx] : dirs_4d) {
        Index ww = w + dw, zz = z + dz, yy = y + dy, xx = x + dx;
        if (0 <= ww && ww < mx.dim1() &&
            0 <= zz && zz < mx.dim2() &&
            0 <= yy && yy < mx.dim3() &&
            0 <= xx && xx < mx.dim4()) {

            if (mx(ww,zz,yy,xx) == '#')
                ++count;
        }
    }
    return count;
}

auto advance_cycle(const Matrix<char,4>& mx)
{
    auto mnext = mx;
    for (Index w = 0; w < mx.dim1(); ++w)
        for (Index z = 0; z < mx.dim2(); ++z)
            for (Index y = 0; y < mx.dim3(); ++y)
                for (Index x = 0; x < mx.dim4(); ++x) {
                    //const auto ch = mx(w,x,y,z);
                    const auto ch = mx(w,z,y,x);
                    switch(ch) {
                    case '.':
                        if (count_adjacents(mx, w, z, y, x) == 3) {
                            mnext(w,z,y,x) = '#';
                        }
                        break;
                    case '#':
                        {
                            auto res = count_adjacents(mx, w, z, y, x);
                            if (res < 2 || 3 < res)
                                mnext(w,z,y,x) = '.';
                            break;
                        }
                    }
                }
    return mnext;
}

auto count_active(const Matrix<char,4>& mx)
{
    return std::count(mx.data(), mx.data() + mx.size(), '#');
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// DEBUG
auto count_active(const Matrix<char,2>& mx)
{
    return std::count(mx.data(), mx.data() + mx.size(), '#');
}

void print_4D_matrix(const Matrix<char,4>& mx)
    // only prints 2D slices that have active cells
{
    for (Index i = 0; i < mx.dim1(); ++i)
        for (Index j = 0; j < mx.dim2(); ++j)
            if (count_active(mx[i][j]) > 0)
                std::cout << "w=" << i << ", z=" << j << '\n'
                          << mx[i][j] << '\n';

}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
try {
    Timer t {};         // start of timing
    std::cout << "Advent of Code 2020\n"
              << "Day 17 - Conway's Cubes\n\n";

    const auto input = read_input_lines();
    Matrix<char,2> mx2 = parse_2d_input(input);
    Matrix<char,3> mx3 = convert_to_3d(mx2, '.');
    auto mx3_big = grow_cube(mx3, '.', 12);
    Matrix<char,4> mx4 = convert_to_4d(mx3_big, '.');

    constexpr int CYCLES = 6;
    for (Index i = 0; i < CYCLES; ++i)
        mx3_big = advance_cycle(mx3_big);

    for (Index i = 0; i < CYCLES; ++i)
        mx4 = advance_cycle(mx4);

    const auto part1 = count_active(mx3_big);
    const auto part2 = count_active(mx4);
    std::cout << std::setw(WIDTH) << "Part 1: " << part1 << '\n';
    std::cout << std::setw(WIDTH) << "Part 2: " << part2 << '\n';

    t.end(WIDTH);       // end of timing, print report
}
catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
