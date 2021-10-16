#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <memory>

#include <get_input.hpp>

const int int_max = std::numeric_limits<int>::max();

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Point {
    int x, y;

    std::vector<Point> get_adjacents() const;

    bool is_adjacent(const Point& pt) const { return distance(pt) == 1; }
    int distance(const Point& pt) const
        { return std::abs(x - pt.x) + std::abs(y - pt.y); }

    void move_to(const Point& pt) { x = pt.x; y = pt.y; }
};

std::vector<Point> Point::get_adjacents() const
{
    std::vector<Point> vpoints;

    vpoints.emplace_back(Point{x, y-1});    // up
    vpoints.emplace_back(Point{x-1, y});    // left
    vpoints.emplace_back(Point{x+1, y});    // right
    vpoints.emplace_back(Point{x, y+1});    // down

    return vpoints;
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point& a, const Point& b)
{
    return !(a == b);
}

bool operator<(const Point& a, const Point& b)
{
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

std::ostream& operator<<(std::ostream& os, const Point& pt)
{
    return os << '(' << pt.x << ", " << pt.y << ')';
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Dist_map {
private:
    std::vector<std::vector<int>> dist_map;
public:
    Dist_map(const std::vector<std::string>& battle_map);

    void print_grid() const;
    void map_distances(const Point& pt);
    Point find_1(const Point& pt) const;

    int operator()(const Point& p) const { return dist_map[p.y][p.x]; }
private:
    void map_adjacents(const Point& pt, int n);
};

Dist_map::Dist_map(const std::vector<std::string>& battle_map)
{
    dist_map.resize(battle_map.size());

    for (auto& vi : dist_map)
        vi.resize(battle_map.front().size());

    for (size_t i = 0; i < battle_map.size(); ++i)
        for (size_t j = 0; j < battle_map[i].size(); ++j)
            dist_map[i][j] = battle_map[i][j] == '.' ? int_max : -1;
}

void Dist_map::print_grid() const
{
    for (const auto& row : dist_map) {
        for (const auto i : row)
            std::cout << i << '\t';
        std::cout << '\n';
    }
}

void Dist_map::map_distances(const Point& pt)
{
    dist_map[pt.y][pt.x] = 0;

    map_adjacents(pt, 1);
}

Point Dist_map::find_1(const Point& pt) const
{
    int dist = (*this)(pt);
    if (dist == 1)
        return pt;

    auto vadj = pt.get_adjacents();         // returns points in reading order
    auto it = std::find_if(std::begin(vadj), std::end(vadj),
            [this, dist](const auto& pt) {
                return (*this)(pt) == dist - 1;
            });

    return find_1(*it);
}

void Dist_map::map_adjacents(const Point& pt, int n)
{
    auto adjacents = pt.get_adjacents();

    std::vector<std::pair<bool,Point>> bool_adjs;
    for (const auto adj : adjacents)
        bool_adjs.emplace_back(std::make_pair(false, std::move(adj)));

    for (auto& adj : bool_adjs) {
        int x = adj.second.x;
        int y = adj.second.y;
        if (0 <= x && x < int(dist_map.front().size()) && 0 <= y && 
                y < int(dist_map.size())) { // check if valid point
            int* pi = &dist_map[y][x];      // get current distance value
            if (0 < *pi && n < *pi) {       // if 'open' and greater than n..
                *pi = n;                    // ..set value to n..
                adj.first = true;           // and mark it to be mapped
            }
        }
    }

    for (auto& adj : bool_adjs)
        if (adj.first)
            map_adjacents(adj.second, n + 1);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Unit;

class Battle {
private:
    int rounds = 0;
    std::vector<std::string> battle_map;
    std::vector<Unit> units;
public:
    explicit Battle(const std::vector<std::string>& input, int elf_dam = 3);

    void print_grid() const;
    int get_score() const;
    Dist_map get_dist_map(const Point& origin) const;

    bool run_simulation();
    std::vector<Unit*> get_targets(const char type);

    void write_move(const Point& from, const Point& to, const char token);
    void write_out(const Point& pt) { battle_map[pt.y][pt.x] = '.'; }
private:
    bool process_turn();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Unit {
private:
    char token;
    Point location;
    int hp = 200;
    Battle* pb;
    int atk_pwr;
public:
    Unit(char tt, int xx, int yy, Battle* pbattle, int pwr = 3)
        : token{tt}, location{xx, yy}, pb{pbattle}, atk_pwr{pwr} { }

    Point get_loc()  const { return location; }
    char  get_tok()  const { return token; }
    int   get_hp()   const { return hp; }
    bool  is_alive() const { return hp > 0 ? true : false; }

    bool take_turn();
    void move(const std::vector<Unit*>& vu, const Dist_map& dm);
    void attack(Unit& u) const;
    void take_damage(int damage);
private:
    std::vector<Unit*>::iterator check_adj(std::vector<Unit*>& targets) const;
    Point get_target_square(const std::vector<Unit*>& tars,
                            const Dist_map& dmap) const;
    Unit* get_weakest_adjacent(std::vector<Unit*>& tars) const;
};

std::ostream& operator<<(std::ostream& os, const Unit& u)
{
    return os << u.get_tok() << '@' << u.get_loc()
              << '(' << u.get_hp() << ')';
}

void Unit::attack(Unit& u) const
{
    u.take_damage(atk_pwr);
}

bool Unit::take_turn()
{
    auto targets = pb->get_targets(token);
    if (targets.empty())
        return false;                               // end of battle!

    auto it_adj = check_adj(targets);               // attack if already adj
    if (it_adj != std::end(targets)) {
        auto weakest = get_weakest_adjacent(targets);
        attack(*weakest);
    }
    else {
        auto dist_map = pb->get_dist_map(get_loc());
        move(targets, dist_map);

        it_adj = check_adj(targets);
        if (it_adj != std::end(targets)) {
            auto weakest = get_weakest_adjacent(targets);
            attack(*weakest);
        }
    }

    return true;
}

Unit* Unit::get_weakest_adjacent(std::vector<Unit*>& vptars) const
{
    std::vector<Unit*> vpadj;
    std::copy_if(std::begin(vptars), std::end(vptars),
                 std::back_inserter(vpadj), [this](const auto ptar) {
                     return this->get_loc().is_adjacent(ptar->get_loc());
                 });

    std::sort(std::begin(vpadj), std::end(vpadj),
            [](const auto& pa, const auto& pb) {
                return pa->get_hp() < pb->get_hp();
            });

    return vpadj.front();
}

void Unit::move(const std::vector<Unit*>& vu, const Dist_map& dmap)
{
    Point temp = location;
    auto goal = get_target_square(vu, dmap);

    if (goal == Point{0,0})
        return;

    goal = dmap.find_1(goal);
    location.move_to(goal);

    pb->write_move(temp, get_loc(), get_tok());
}

std::vector<Unit*>::iterator Unit::check_adj(std::vector<Unit*>& targets) const
{
    std::sort(std::begin(targets), std::end(targets),
            [](const auto pa, const auto pb) {
                return pa->get_loc() < pb->get_loc();
            });

    return std::find_if(std::begin(targets), std::end(targets),
            [this](const auto p) {
                return this->get_loc().is_adjacent(p->get_loc());
            });
}

Point Unit::get_target_square(const std::vector<Unit*>& vptargets,
                              const Dist_map& dmap) const
{
    std::vector<Point> all_pts;
    for (const auto ptarget : vptargets) {
        auto vtemp = ptarget->get_loc().get_adjacents();
        std::copy(std::begin(vtemp), std::end(vtemp),
                  std::back_inserter(all_pts));
    }

    all_pts.erase(std::remove_if(std::begin(all_pts), std::end(all_pts),
                [&dmap](const auto& pt) {
                    return dmap(pt) < 0 || dmap(pt) == int_max;
                }), std::end(all_pts));

    if (all_pts.empty())
        return Point{0,0};                  // good use case for std::optional

    auto sm_it = std::min_element(std::begin(all_pts), std::end(all_pts),
            [&dmap](const auto& a, const auto& b) {
                return dmap(a) < dmap(b);
            });

    int smallest = dmap(*sm_it);
    all_pts.erase(std::remove_if(std::begin(all_pts), std::end(all_pts),
                [&dmap, smallest](const auto& pt) {
                    return smallest < dmap(pt);
                }), std::end(all_pts));

    std::sort(std::begin(all_pts), std::end(all_pts));
    
    return all_pts.front();
}

void Unit::take_damage(int damage)
{
    hp -= damage;
    if (hp <= 0)
        pb->write_out(get_loc());
}

bool operator<(const Unit& a, const Unit& b)
{
    return a.get_loc() < b.get_loc();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Battle::Battle(const std::vector<std::string>& input, int elf_dam)
    : battle_map{input}
{
    for (size_t y = 0; y < battle_map.size(); ++y)
        for (size_t x = 0; x < battle_map[y].size(); ++x) {
            switch (battle_map[y][x]) {
            case 'E':
                units.emplace_back(Unit{'E', int(x), int(y), this, elf_dam});
                break;
            case 'G':
                units.emplace_back(Unit{'G', int(x), int(y), this});
                break;
            default:
                // do nothing for #'s and .'s
                break;
            }
        }
}

void Battle::print_grid() const
{
    for (const auto& row : battle_map) {
        for (const auto ch : row)
            std::cout << ch;
        std::cout << '\n';
    }
    std::cout << '\n';
}

int Battle::get_score() const
{
    return rounds * std::accumulate(std::begin(units), std::end(units), 0,
                [](auto sum, auto& u) { return sum + u.get_hp(); });
}

bool Battle::run_simulation()
{
    const auto num_elves = std::count_if(std::begin(units), std::end(units),
            [](const auto& u) { return u.get_tok() == 'E'; });

    while (process_turn()) {
        ++rounds;
    }

    // remove deceased units
    units.erase(std::remove_if(std::begin(units), std::end(units),
                [](auto& u) { return !u.is_alive(); }),
            std::end(units));

    // part 2: check on elves..
    auto cur_elves = std::count_if(std::begin(units), std::end(units),
            [](const auto& u) { return u.get_tok() == 'E' && u.is_alive(); });

    if (cur_elves == num_elves)
        return true;
    return false;
}

void Battle::write_move(const Point& from, const Point& to, const char token)
{
    battle_map[from.y][from.x] = '.';
    battle_map[to.y][to.x] = token;
}

Dist_map Battle::get_dist_map(const Point& origin) const
{
    auto dist_map = Dist_map{battle_map};

    dist_map.map_distances(origin);

    return dist_map;
}

std::vector<Unit*> Battle::get_targets(const char type)
{
    std::vector<Unit*> targets;

    for (auto& unit : units)
        if (unit.get_tok() != type && unit.is_alive())
            targets.push_back(&unit);

    return targets;
}

bool Battle::process_turn()
{
    std::sort(std::begin(units), std::end(units));

    for (auto& unit : units)
        if (unit.is_alive() && !unit.take_turn())   // false take_turn ends
            return false;                           // battle

    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 15 - Beverage Bandits\n";

    auto input = utils::get_input_lines(argc, argv, "15");

    auto elf_pwr = 3;
    auto battle = std::make_unique<Battle>(input, elf_pwr);

    battle->run_simulation();
    auto part1 = battle->get_score();
    std::cout << "Part 1: " << part1 << '\n';

    do {
        auto temp = std::make_unique<Battle>(input, ++elf_pwr);
        battle = std::move(temp);
    } while(!(battle->run_simulation()));

    auto part2 = battle->get_score();
    std::cout << "Part 2: " << part2 << '\n';
}
