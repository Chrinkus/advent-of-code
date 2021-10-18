#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <tuple>
#include <cmath>
#include <numeric>
#include <array>

class Moon {
public:
    Moon() = delete;
    Moon(const std::string& s);

    int x() const { return std::get<0>(pos); }
    int y() const { return std::get<1>(pos); }
    int z() const { return std::get<2>(pos); }

    int vx() const { return std::get<0>(vel); }
    int vy() const { return std::get<1>(vel); }
    int vz() const { return std::get<2>(vel); }

    int calculate_energy() const;

    void apply_gravity(const Moon& m);
    void apply_velocity();

private:
    std::tuple<int,int,int> pos;
    std::tuple<int,int,int> vel;
};

Moon::Moon(const std::string& s)
{
    static const std::regex pat {R"(^<x=(-?\d+), y=(-?\d+), z=(-?\d+)>$)"};
    std::smatch match;
    std::regex_match(s, match, pat);
    int x = std::stoi(match[1].str());
    int y = std::stoi(match[2].str());
    int z = std::stoi(match[3].str());

    pos = std::make_tuple(x, y, z);
    vel = std::make_tuple(0, 0, 0);
}

int Moon::calculate_energy() const
{
    int pot = std::abs(std::get<0>(pos)) +
              std::abs(std::get<1>(pos)) +
              std::abs(std::get<2>(pos));

    int kin = std::abs(std::get<0>(vel)) +
              std::abs(std::get<1>(vel)) +
              std::abs(std::get<2>(vel));

    return pot * kin;
}

void Moon::apply_gravity(const Moon& m)
{
    if (x() < m.x())
        ++std::get<0>(vel);
    else if (x() > m.x())
        --std::get<0>(vel);

    if (y() < m.y())
        ++std::get<1>(vel);
    else if (y() > m.y())
        --std::get<1>(vel);

    if (z() < m.z())
        ++std::get<2>(vel);
    else if (z() > m.z())
        --std::get<2>(vel);
}

void Moon::apply_velocity()
{
    std::get<0>(pos) += std::get<0>(vel);
    std::get<1>(pos) += std::get<1>(vel);
    std::get<2>(pos) += std::get<2>(vel);
}

std::ostream& operator<<(std::ostream& os, const Moon& m)
{
    return os << "x: " << m.x() << " y: " << m.y() << " z: " << m.z();
}

void time_steps(std::vector<Moon>& vm, int n)
{
    for (auto i = 0; i < n; ++i) {
        for (auto& a : vm)
            for (const auto& b : vm) {
                if (&a == &b)
                    continue;
                a.apply_gravity(b);
            }

        for (auto& m : vm)
            m.apply_velocity();
    }
}

struct Coord {
    int c;
    int v;
};

bool operator==(const Coord& a, const Coord& b)
{
    return a.c == b.c && a.v == b.v;
}

void apply_gravity(std::array<Coord,4>& ac)
{
    for (auto& a : ac)
        for (const auto& b : ac) {
            if (&a == &b)
                continue;
            if (a.c < b.c)
                ++a.v;
            else if (a.c > b.c)
                --a.v;
        }
}

void apply_velocity(std::array<Coord,4>& ac)
{
    for (auto& c : ac)
        c.c += c.v;
}

int64_t run_to_repeat(const std::array<Coord,4>& ac)
{
    int64_t steps = 0;
    std::array<Coord,4> acpy = ac;
    while (true) {
        apply_gravity(acpy);
        apply_velocity(acpy);
        ++steps;
        if (ac == acpy)
            return steps;
    }
}

int64_t steps_to_repeat(const std::vector<Moon>& vm)
{
    std::array<Coord,4> ax;
    std::array<Coord,4> ay;
    std::array<Coord,4> az;

    for (auto i = 0; i < vm.size(); ++i) {
        ax[i] = Coord{vm[i].x(), vm[i].vx()};
        ay[i] = Coord{vm[i].y(), vm[i].vy()};
        az[i] = Coord{vm[i].z(), vm[i].vz()};
    }

    int64_t x_steps = run_to_repeat(ax);
    int64_t y_steps = run_to_repeat(ay);
    int64_t z_steps = run_to_repeat(az);

    return std::lcm(x_steps, std::lcm(y_steps, z_steps));
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 12\n"
              << "The N-Body Problem\n";

	std::vector<std::string> vs;
	for (std::string line; std::getline(std::cin, line); )
		vs.push_back(line);

    std::vector<Moon> vm1;
    for (const auto& s : vs)
        if (!s.empty())
            vm1.emplace_back(Moon{s});

    std::vector<Moon> vm2 = vm1;        // unmutated copy for part 2

    time_steps(vm1, 1000);

    auto part1 = std::accumulate(std::begin(vm1), std::end(vm1), 0,
            [](const auto& sum, const auto& moon) {
                return sum + moon.calculate_energy();
            });

	auto part2 = steps_to_repeat(vm2);

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
