#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

int get_odd_square(int input)
{
    double square = sqrt(double(input));
    int ceiled = int(ceil(square));
    int odd_square = ceiled % 2 == 0 ? ceiled + 1 : ceiled;
    return odd_square;
}

int get_steps(int input)
{
    int side = get_odd_square(input);           // number of vals / side
    int min_steps = int(side / 2.0);            // middle of side

    int max = pow(side, 2);                     // max val of this level
    int diff = max - input;

    int small = side - 1;                       // distance between corners
    int location = int(diff / small);           // # of sides away from max
    int corner = max - small * location;        // nearest larger corner
    int dist = corner - input;                  // distance from corner
    int add_steps = abs(min_steps - dist);      // distance from middle

    int steps = min_steps + add_steps;

    return steps;
}

int main()
try {
    cout << "Enter memory location to count steps from:\n";
    int input;
    cin >> input;

    int steps = get_steps(input);

    cout << "The number of memory steps from " << input
         << " is: " << steps << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
