#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <numeric>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class Measure {     // a quantity of a given chemical
public:
    Measure() = default;
    Measure(int64_t qty, const std::string& chem)
        : q{qty}, c{chem} { }

    int64_t qty() const { return q; }
    const std::string& chem() const { return c; }
private:
    int64_t q;
    std::string c;
};

std::ostream& operator<<(std::ostream& os, const Measure& m)
{
    return os << m.qty() << ' ' << m.chem();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
using Iter = std::vector<Measure>::const_iterator;

class Reaction {    // a measure (yield) of a combination of chemicals
public:
    Reaction() = default;
    Reaction(Measure mm, std::vector<Measure> vm)
        : m{std::move(mm)}, input{std::move(vm)} { }

    int64_t yield() const { return m.qty(); }
    std::string chem() const { return m.chem(); }

    void print() const;

    Iter begin() const { return std::begin(input); }
    Iter end()   const { return std::end(input); }

private:
    Measure m;
    std::vector<Measure> input;
};

void Reaction::print() const
{
    std::cout << m << '\n';
    for (const auto& mm : input)
        std::cout << "  " << mm << '\n';
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Reaction parse_reaction(const std::string& s)
{
    std::istringstream iss {s};

    std::vector<Measure> vm;
    int64_t qty;
    std::string chem;
    while (iss >> qty >> chem) {
        if (chem.back() == ',')
            chem.pop_back();
        vm.emplace_back(Measure{qty, chem});
    }
    iss.clear();
    std::string sep;
    iss >> sep >> qty >> chem;
    if (sep != "=>")
        std::cerr << "Bad separator: " << sep << '\n';
    return Reaction(Measure{qty,chem}, vm);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class Nanofactory; 

struct Stock {
    int64_t need = 0;
    int64_t spare = 0;
    int64_t total() const { return need + spare; }
};

class Order {       // an inventory of the chemicals needed for a measure
public:
    Order() = delete;
    explicit Order(const Nanofactory& nanofactory)
        : nf{nanofactory} { }

    int64_t get_qty(const std::string& chem) const;

    void add(const Measure& m);
private:
    const Nanofactory& nf;
    std::unordered_map<std::string,Stock> counts;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class Nanofactory { // reactions are read from here
public:
    Nanofactory() = delete;
    explicit Nanofactory(const std::vector<std::string>& input);

    Order place_order(const Measure& m) const;
    Order max_fuel(int64_t ore, int64_t ore_per_1 = 1) const;
    const Reaction& get_reaction(const std::string& s) const;
private:
    std::unordered_map<std::string,Reaction> reactions;
};

Nanofactory::Nanofactory(const std::vector<std::string>& input)
{
    for (const auto& s : input) {
        Reaction r = parse_reaction(s);
        reactions[r.chem()] = std::move(r);
    }
}

Order Nanofactory::place_order(const Measure& m) const
{
    Order o {*this};
    o.add(m);
    return o;
}

Order Nanofactory::max_fuel(int64_t ore, int64_t ore_per_1) const
{
    Order o {*this};
    int64_t start = ore / ore_per_1;

    do {
        o.add(Measure{start, "FUEL"});
        double ore_left = ore - o.get_qty("ORE");
        start = std::ceil(ore_left / ore_per_1);
    } while (o.get_qty("ORE") < ore);

    return o;
}

const Reaction& Nanofactory::get_reaction(const std::string& s) const
{
    return reactions.at(s);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int64_t Order::get_qty(const std::string& chem) const
{
    return counts.at(chem).total();
}

void Order::add(const Measure& m)
{
    Stock& st = counts[m.chem()];
    st.need += m.qty();

    if (m.chem() == "ORE")
        return;                         // end recursion

    if (m.qty() <= st.spare) {
        st.spare -= m.qty();
        return;                         // no need to produce more
    }

    int64_t need_now = m.qty() - st.spare;
    const Reaction& r = nf.get_reaction(m.chem());
    int64_t mul = std::ceil(static_cast<double>(need_now) / r.yield());
    st.spare = mul * r.yield() - need_now;

    for (const auto& mm : r) {
        add(Measure{mul * mm.qty(), mm.chem()});
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
    std::cout << "Advent of Code 2019 - Day 14\n"
              << "Space Stoichiometry\n";

	std::vector<std::string> vs;
    for (std::string line; std::getline(std::cin, line); )
        if (!line.empty())
            vs.emplace_back(line);

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
    Nanofactory nf {vs};
    Order order1 = nf.place_order(Measure{1, "FUEL"});

	auto part1 = order1.get_qty("ORE");
	std::cout << "Part 1: " << part1 << '\n';

    constexpr int64_t max_ore = 1'000'000'000'000;
    Order order2 = nf.max_fuel(max_ore, part1);

	auto part2 = order2.get_qty("FUEL") - 1;
	std::cout << "Part 2: " << part2 << '\n';
}
