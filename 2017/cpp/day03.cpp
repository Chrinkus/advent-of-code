#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

using namespace std;

// Incremental spiral
struct Spiral_inc {
    explicit Spiral_inc(int);             // create the spiral level w/t

    int get_corner(int);

    int side;
    int small;
    int max;
    int min;
};

Spiral_inc::Spiral(int t)
{
    side = ceil(sqrt(t));
    if (side % 2 == 0) ++side;
    small = side - 1;
    min = pow(side - 2, 2) + 1;
    max = pow(side, 2);
}

int Spiral_inc::get_corner(int n)
{
    int corner = min - 1;
    while (corner < n)
        corner += small;

    return corner;
}

int spiral_steps_from(int input)
{
    Spiral_inc sp {input};

    int min_steps = int(sp.side / 2.0);
    int corner = sp.get_corner(input);
    int dist = corner - input;
    int add_steps = abs(min_steps - dist);

    return min_steps + add_steps;
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "289326";
    int target = stoi(input);

    int part_1 = spiral_steps_from(target);

    cout << "The answer to part 1 is: " << part_1 << '\n';
    //cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
