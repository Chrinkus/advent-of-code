#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;

int find_next_biggest(int target)
    // change this to a grammar? how to handle so many variables..
{
    vector<int> prev {1, 2, 4, 5, 10, 11, 23, 25};      // fake second level

    int width = 5;
    while (true) {              // each iteration is a new level
        int small = width - 1;
        int size = pow(width, 2) - pow(width - 2, 2);

        vector<int> curr (size);

        int c = 0;
        int p = 1;
        // calculate each side in a loop
        for (int s = 0; s < 4; ++s) {

            // the run
            for (int m = 0; m < small - 2; ++m) {
                int val = 0;

                // first side only
                if (c < 2) {
                    if (c == 0)
                        val = prev.back() + prev.front();
                    else
                        val = curr[c-1] + prev.back() + prev.front() + prev[p];
                }
                // sides 2 - 4
                else if (m == 0) {
                    val = curr[c-1] + curr[c-2] + prev[p] + prev[p+1];
                    ++p;
                }
                else {
                    val = curr[c-1] + prev[p-1] + prev[p] + prev[p+1];
                    ++p;
                }
                
                if (val > target) return val;

                curr[c] = val;
                ++c;
            }

            // the turn - pivot on p, no increment
            int bend = c == size - 2        // check for last side
                    ? curr[c-1] + prev[p-1] + prev[p] + curr[0]
                    : curr[c-1] + prev[p-1] + prev[p];

            if (bend > target)
                return bend;
            else {
                curr[c] = bend;
                ++c;
            }

            int corner = c == size - 1      // check for last side
                    ? curr[c-1] + prev[p] + curr[0]
                    : curr[c-1] + prev[p];

            if (corner > target)
                return corner;
            else {
                curr[c] = corner;
                ++c;
            }
        }

        prev = curr;
        width += 2;
    }

    throw runtime_error("Somehow made it through infinite loop");
}

int main()
try {
    // code
    cout << "Enter a value to find next largest in sequence:\n";
    int val;
    cin >> val;

    int next = find_next_biggest(val);
    cout << "The next largest in sequence is: " << next << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
