#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#include "Intcode_vm.hpp"

struct Phase {
    std::string sequence;
    int output;
    Phase(const std::string& s, int o)
        : sequence{s}, output{o} { }
};

bool operator<(const Phase& a, const Phase& b)
{
    return a.output < b.output;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 7\n"
              << "Amplification Circuit\n";

    std::string input;
    std::cin >> input;

    std::string phases = "01234";

    std::stringstream ss;
    std::vector<Phase> vp;
    do {
        int signal = 0;
        // construct fresh vms
        std::vector<Intcode_vm> vvm;
        for (auto i = 0; i < 5; ++i)
            vvm.emplace_back(Intcode_vm{input});
        // run permutation
        auto it = std::begin(phases);
        for (auto& vm : vvm) {
            ss << *it++ << ' ' << signal << ' ';
            vm.run(ss, ss);
            ss >> signal;
        }
        vp.emplace_back(Phase{phases, signal});
    } while (std::next_permutation(std::begin(phases), std::end(phases)));

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

    std::string phases2 = "56789";
    std::vector<Phase> vp2;
    do {
        int signal = 0;
        std::stringstream etoa;
        std::stringstream atob;
        std::stringstream btoc;
        std::stringstream ctod;
        std::stringstream dtoe;
        Intcode_vm vma {input};
        Intcode_vm vmb {input};
        Intcode_vm vmc {input};
        Intcode_vm vmd {input};
        Intcode_vm vme {input};
        etoa << phases2[0] << ' ' << signal << ' ';
        atob << phases2[1] << ' ';
        btoc << phases2[2] << ' ';
        ctod << phases2[3] << ' ';
        dtoe << phases2[4] << ' ';

        while (!vma.halted() && !vmb.halted() && !vmc.halted() &&
                !vmd.halted() && !vme.halted()) {
            vma.run(etoa, atob);
            vmb.run(atob, btoc);
            vmc.run(btoc, ctod);
            vmd.run(ctod, dtoe);
            vme.run(dtoe, etoa);
        }

        etoa >> signal;
        vp2.emplace_back(Phase{phases2, signal});
    } while (std::next_permutation(std::begin(phases2), std::end(phases2)));

	auto part1 = std::max_element(std::begin(vp), std::end(vp))->output;
	auto part2 = std::max_element(std::begin(vp2), std::end(vp2))->output;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
