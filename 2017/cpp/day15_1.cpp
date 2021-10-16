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

    for (int i = 0; i < 40000000; ++i) {
        A = A * factor_A % INT_MAX;
        B = B * factor_B % INT_MAX;

        bitset<16> a (A);
        bitset<16> b (B);

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
