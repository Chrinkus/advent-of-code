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
    Measure(int qty, const std::string& chem)
        : q{qty}, c{chem} { }

    int qty() const { return q; }
    const std::string& chem() const { return c; }
private:
    int q;
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

    int yield() const { return m.qty(); }
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
    int qty;
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

class Order {       // an inventory of the chemicals needed for a measure
public:
    Order() = delete;
    explicit Order(const Nanofactory& nanofactory)
        : nf{nanofactory} { }

    int get_qty(const std::string& chem) const { return counts.at(chem); }

    void add(const Measure& m);
    void remove(const Measure& m);
private:
    const Nanofactory& nf;
    std::unordered_map<std::string,int> counts;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class Nanofactory { // reactions are read from here
public:
    Nanofactory() = delete;
    explicit Nanofactory(const std::vector<std::string>& input);

    Order place_order(const Measure& m) const;
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

const Reaction& Nanofactory::get_reaction(const std::string& s) const
{
    return reactions.at(s);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void Order::add(const Measure& m)
{
    int& cur = counts[m.chem()];
    int old = cur;                  // save old count
    cur += m.qty();                   // add new count
    if (m.chem() == "ORE")
        return;                         // end recursion

    const Reaction& r = nf.get_reaction(m.chem());

    if (old != 0) {
        int br = std::ceil(static_cast<double>(old) / r.yield());
        for (const auto& mm : r)
            remove(Measure{br * mm.qty(), mm.chem()});
    }

    int ba = std::ceil(static_cast<double>(cur) / r.yield());
    for (const auto& mm : r) {
        add(Measure{ba * mm.qty(), mm.chem()});
    }
}

void Order::remove(const Measure& m)
{
    counts[m.chem()] -= m.qty();
    if (m.chem() == "ORE")
        return;                         // end recursion

    const Reaction& r = nf.get_reaction(m.chem());
    int br = std::ceil(static_cast<double>(m.qty()) / r.yield());
    for (const auto& mm : r) {
        remove(Measure{br * mm.qty(), mm.chem()});
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
    Order order = nf.place_order(Measure{1, "FUEL"});

	auto part1 = order.get_qty("ORE");
	auto part2 = 0;

	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
