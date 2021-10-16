#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
try {
    /*
    // Registers
    int a = 1;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;

    // Instructions
    b = 79;
    c = b;
    if (a != 0) {
        b *= 100;
        b += 100000;
        c = b;
        c += 17000;
    }
    do {
        f = 1;
        d = 2;
        do {
            e = 2;
            do {
                g = d;              // d == 2 and increments
                g *= e;             // e == 2 and increments
                g -= b;             // b == 107900 and incs by 17
                if (g == 0) f = 0;  // if the result of d*e == b flip f
                e += 1;
                g = e;
                g -= b; 
            } while (g != 0);       // when e incs up to b leave loop
            d += 1;
            g = d;
            g -= b;
        } while (g != 0);           // when d incs up to b leave loop
        if (f == 0) h += 1;         // increase h when d*e does not equal b
        g = b;
        g -= c;
        if (g == 0) break;          // when b == c leave entire loop
        b += 17;                    // increase b by 1/1000 of the diff w/c
    } while (g != 0);
    */

    // Sieve of Eratosthenes
    vector<int> primes;
    vector<int> sieve;

    // create the sieve and fill the primes
    for (int i = 0; i <= 124900; ++i) 
        if (i < 2)
            sieve.push_back(0);
        else
            sieve.push_back(i);

    for (int i = 0; i < sieve.size(); ++i) {
        if (sieve[i] != 0) {
            primes.push_back(i);

            if (i * i < sieve.size())
                for (int j = i; j < sieve.size(); j += i)
                    sieve[j] = 0;
        }
    }

    // AoC solution
    int h = 0;

    for (int i = 107900; i <= 124900; i += 17)
        if (sieve[i] == 0)
            ++h;

    cout << "Register h ends with: " << h << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
