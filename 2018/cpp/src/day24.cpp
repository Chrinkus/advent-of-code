#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <memory>

#include <get_input.hpp>

class Bad_regex{};

class Group {
public:
    enum class Type { radiation, slashing, bludgeoning, fire, cold, };
private:
    int num_units;
    int hp;
    int atk_pwr;
    int initiative;
    Type atk_type;
    std::vector<Type> weaknesses;
    std::vector<Type> immunities;
    Group* target = nullptr;
public:
    explicit Group(const std::string& input);

    // part 2
    void boost_atk(int n) { atk_pwr += n; }

    // for ostream
    int get_hp() const { return hp; }
    int get_atk() const { return atk_pwr; }

    int effective_pwr() const { return num_units * atk_pwr; }
    int init() const { return initiative; }
    int units() const { return num_units; }
    Type get_type() const { return atk_type; }

    bool is_weak_to(const Type& t) const;
    bool is_immune_to(const Type& t) const;

    int check_attack(const Group& target) const;
    Group* get_target() const { return target; }
    void set_target(Group* pg) { target = pg; }
    void attack();
    void take_damage(int damage) { num_units -= damage / hp; }
};

std::ostream& operator<<(std::ostream& os, const Group& g)
{
    static const std::vector<std::string> type_names {
        "radiation", "slash", "bludgeon", "fire", "cold"
    };

    return os << "Type: " << type_names[int(g.get_type())] << '\t'
              << g.units() << " units, " << g.get_hp() << " hp, "
              << g.get_atk() << " atk, "
              << g.init() << " init.";
}

bool operator>(const Group& a, const Group& b)
{
    return a.effective_pwr() > b.effective_pwr() ||
           (a.effective_pwr() == b.effective_pwr() && a.init() > b.init());
}

bool Group::is_weak_to(const Type& t) const
{
    auto it = std::find(std::begin(weaknesses), std::end(weaknesses), t);
    return it == std::end(weaknesses) ? false : true;
}

bool Group::is_immune_to(const Type& t) const
{
    auto it = std::find(std::begin(immunities), std::end(immunities), t);
    return it == std::end(immunities) ? false : true;
}

int Group::check_attack(const Group& target) const
{
    int mult = 1;
    if (target.is_weak_to(atk_type))
        mult = 2;
    else if (target.is_immune_to(atk_type))
        mult = 0;

    return mult;
}

void Group::attack()
{
    if (target)
        target->take_damage(check_attack(*target) * effective_pwr());
}

Group::Group(const std::string& input)
{
    const std::regex base {R"(^(\d+).*with (\d+).*does (\d+) (\w+) dam.* (\d+)$)"};
    const std::regex immu_pat {R"(\(?immune to ([\w\s,]+)(?:;|\)))"};
    const std::regex weak_pat {R"(\(?weak to ([\w\s,]+)(?:;|\)))"};

    static const std::unordered_map<std::string,Type> type_map {
        { "radiation", Type::radiation },
        { "slashing", Type::slashing },
        { "bludgeoning", Type::bludgeoning },
        { "fire", Type::fire },
        { "cold", Type::cold }
    };

    std::smatch matches;
    if (std::regex_match(input, matches, base)) {
        num_units = std::stoi(matches[1]);
        hp = std::stoi(matches[2]);
        atk_pwr = std::stoi(matches[3]);
        atk_type = type_map.at(matches[4].str());
        initiative = std::stoi(matches[5]);
    } else {
        throw Bad_regex{};
    }

    if (std::regex_search(input, matches, immu_pat)) {
        std::istringstream iss { matches[1] };
        for (std::string s; std::getline(iss, s, ','); ) {
            immunities.push_back(type_map.at(s));
            iss.ignore(1, ' ');
        }
    }
    if (std::regex_search(input, matches, weak_pat)) {
        std::istringstream iss { matches[1] };
        for (std::string s; std::getline(iss, s, ','); ) {
            weaknesses.push_back(type_map.at(s));
            iss.ignore(1, ' ');
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Battle {
private:
    std::vector<Group> immune_sys;
    std::vector<Group> infection;
public:
    explicit Battle(const std::vector<std::string>& input);

    int simulate_immuno_war();
    void boost_immune(int n);
    int get_immune_sum() const { return sum_units(immune_sys); }
private:
    void fight();
    void select_targets(std::vector<Group>& vatk, std::vector<Group>& vdef);
    void attack();
    void purge_dead();
    int sum_units(const std::vector<Group>& vg) const;
    std::vector<Group*> get_all();
    bool deadlocked();
};

Battle::Battle(const std::vector<std::string>& input)
{
    auto it = ++std::begin(input);              // skip 'Immune System:'
    while (!it->empty()) {
        immune_sys.emplace_back(Group(*it++));
    }

    ++it;                                       // skip 'Infection:'
    while (++it != std::end(input))
        infection.emplace_back(Group(*it));

    /*
    std::cout << "Immune system:\n";
    for (const auto& g : immune_sys)
        std::cout << '\t' << g << '\n';
    std::cout << "Infection:\n";
    for (const auto& g : infection)
        std::cout << '\t' << g << '\n';
    */
}

int Battle::simulate_immuno_war()
{
    while (!immune_sys.empty() && !infection.empty()) {
        fight();
        std::cout << "Immune system: " << sum_units(immune_sys)
                  << " Infection: " << sum_units(infection) << '\n';
        if (deadlocked())
            return 0;
    }

    return immune_sys.empty() ? sum_units(infection) : sum_units(immune_sys);
}

void Battle::boost_immune(int n)
{
    for (auto& g : immune_sys)
        g.boost_atk(n);

}

void Battle::fight()
{
    select_targets(immune_sys, infection);
    select_targets(infection, immune_sys);
    attack();
    purge_dead();
}

void Battle::select_targets(std::vector<Group>& vatk, std::vector<Group>& vdef)
{
    std::vector<Group*> vpa;
    for (auto& g : vatk)
        vpa.push_back(&g);

    std::sort(std::begin(vpa), std::end(vpa),
            [](const auto& pa, const auto& pb) { return *pa > *pb; });
    
    std::vector<Group*> vpd;               // then gather pointers to targets
    for (auto& g : vdef)
        vpd.push_back(&g);
    
    for (auto& patk : vpa) {                // each atk grp picks a target
        if (vpd.empty())                   // until no targets are left
            patk->set_target(nullptr);
        else {
            std::sort(std::rbegin(vpd), std::rend(vpd),
                [&patk](const auto pa, const auto pb) {
                    return patk->check_attack(*pa) > patk->check_attack(*pb) ||
                        (patk->check_attack(*pa) == patk->check_attack(*pb) &&
                           *pa > *pb);
                });
            auto ptar = vpd.back();          
            if (patk->check_attack(*ptar) > 0) {  
                patk->set_target(ptar);           
                vpd.pop_back();                
            } else {
                patk->set_target(nullptr);       
            }
        }
    }
}

void Battle::attack()
{
    auto vall = get_all();

    std::sort(std::begin(vall), std::end(vall),
            [](const auto pa, const auto pb) {
                return pa->init() > pb->init();
            });

    for (auto& pg : vall)
        if (pg->units() > 0)        // during battle groups may be eliminated
            pg->attack();
}

void Battle::purge_dead()
{
    immune_sys.erase(std::remove_if(
                std::begin(immune_sys), std::end(immune_sys),
                [](const auto& g) { return g.units() <= 0; }
            ), std::end(immune_sys));
    infection.erase(std::remove_if(
                std::begin(infection), std::end(infection),
                [](const auto& g) { return g.units() <= 0; }
            ), std::end(infection));
}

int Battle::sum_units(const std::vector<Group>& vg) const
{
    return std::accumulate(std::begin(vg), std::end(vg), 0,
            [](auto& sum, const auto& g) { return sum + g.units(); });
}

std::vector<Group*> Battle::get_all() 
{
    std::vector<Group*> vall;
    for (auto& g : immune_sys)
        vall.push_back(&g);
    for (auto& g : infection)
        vall.push_back(&g);
    return vall;
}


bool Battle::deadlocked()
{
    const auto vall = get_all();
    if (std::any_of(std::begin(vall), std::end(vall),
                [](const auto& p) {
                    return p->get_target();
                }))
        return false;
    std::cout << "Deadlocked!\n";
    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int tilt_immune_sys(const std::vector<std::string>& input)
{
    int imm_boost = 43;
    while (true) {
        auto b = std::make_unique<Battle>(input);
        b->boost_immune(imm_boost);
        b->simulate_immuno_war();
        auto imm_units = b->get_immune_sum();
        if (imm_units > 0)
            return imm_units;
        else
            ++imm_boost;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 24 - Immune System Simulator 20XX\n";

    auto input = utils::get_input_lines(argc, argv, "24");
    auto battle = Battle(input);

    auto part1 = battle.simulate_immuno_war();
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = tilt_immune_sys(input);
    std::cout << "Part 2: " << part2 << '\n';
}
