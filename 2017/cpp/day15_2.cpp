#include <iostream>
#include <stdexcept>

using namespace std;

int main()
try {
    constexpr int factor_A = 16807;
    constexpr int factor_B = 48271;

    constexpr int initial_A = 634;
    constexpr int initial_B = 301;

    unsigned long int A = initial_A;
    unsigned long int B = initial_B;
    int count = 0;
    int pairs = 0;

    while (pairs < 5000000) {

        do { 
            A = A * factor_A % INT_MAX;
        } while (A % 4);

        do {
            B = B * factor_B % INT_MAX;
        } while (B % 8);

        bitset<16> a (A);
        bitset<16> b (B);

        ++pairs;

        if (a == b) ++count;
    }

    cout << count << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
