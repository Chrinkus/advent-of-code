#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <get_input.hpp>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Off_tracks {};            // cart off track error
class Ghost_crash{};            // crash with missing cart error
class Bad_orient{};             // somehow tried to make cart out of not a cart

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

enum class Turn { left, straight, right };

Turn operator++(Turn& t)
{
    t = (t == Turn::right) ? Turn::left : Turn(int(t) + 1);
    return t;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

enum class Orientation { up = '^', right = '>', down = 'v', left = '<',
                         crash = 'X' };

Orientation operator++(Orientation& o)
{
    switch (o) {
    case Orientation::up:
        o = Orientation::right;
        break;
    case Orientation::right:
        o = Orientation::down;
        break;
    case Orientation::down:
        o = Orientation::left;
        break;
    case Orientation::left:
        o = Orientation::up;
        break;
    case Orientation::crash:
        break;      // don't change crash
    }
    return o;
}

Orientation operator--(Orientation& o)
{
    switch (o) {
    case Orientation::up:
        o = Orientation::left;
        break;
    case Orientation::right:
        o = Orientation::up;
        break;
    case Orientation::down:
        o = Orientation::right;
        break;
    case Orientation::left:
        o = Orientation::down;
        break;
    case Orientation::crash:
        break;      // don't change crash
    }
    return o;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Cart {
public:
    Cart(int x, int y, Orientation o);

    int x() const { return xx; }
    int y() const { return yy; }
    char look_ahead(std::vector<std::string>& trax) const;
    Orientation facing() const { return ff; }
    char get_curr() const { return current; }

    char turn();
    void advance(std::vector<std::string>& trax, char look_ahead);
    void bend(char ahead);
    void crash() { ff = Orientation::crash; }
    char crash(std::vector<Cart>& carts);
private:
    int xx, yy;
    Orientation ff;
    char current = '\0';            // save track piece
    Turn next_turn = Turn::left;
};

Cart::Cart(int x, int y, Orientation o) : xx{x}, yy{y}, ff{o}
{
    switch (ff) {
    case Orientation::up:
    case Orientation::down:
        current = '|';
        break;
    case Orientation::left:
    case Orientation::right:
        current = '-';
        break;
    default:
        throw Bad_orient{};
    }
}

char Cart::look_ahead(std::vector<std::string>& trax) const
{
    char token = '\0';

    switch (ff) {
    case Orientation::up:       token = trax[yy-1][xx]; break;
    case Orientation::right:    token = trax[yy][xx+1]; break;
    case Orientation::down:     token = trax[yy+1][xx]; break;
    case Orientation::left:     token = trax[yy][xx-1]; break;
    case Orientation::crash:    token = '\0';           break;
    default:                                            break; }

    return token;
}

char Cart::turn()
{
    char next_token = '\0';

    switch (next_turn) {
    case Turn::left:      next_token = char(--ff);  break;
    case Turn::straight:  next_token = char(ff);    break;
    case Turn::right:     next_token = char(++ff);  break;
    default:                                        break; }

    ++next_turn;
    return next_token;
}

void Cart::advance(std::vector<std::string>& trax, char look_ahead)
{
    trax[yy][xx] = current;                     // replace track token
    current = look_ahead;

    switch (ff) {                               // move
    case Orientation::up:     --yy; break;
    case Orientation::right:  ++xx; break;
    case Orientation::down:   ++yy; break;
    case Orientation::left:   --xx; break;
    default:                        break; }

    trax[yy][xx] = static_cast<char>(ff);       // put self on track
}

void Cart::bend(char ahead)
{
    switch (ff) {
    case Orientation::up:       ahead == '/' ? ++ff : --ff;     break;
    case Orientation::right:    ahead == '/' ? --ff : ++ff;     break;
    case Orientation::down:     ahead == '/' ? ++ff : --ff;     break;
    case Orientation::left:     ahead == '/' ? --ff : ++ff;     break;
    default:                                                    break; }
}

char Cart::crash(std::vector<Cart>& carts)
{
    ff = Orientation::crash;

    auto it = std::find_if(std::begin(carts), std::end(carts),
            [this](auto& cart) {
                return cart.facing() != Orientation::crash &&
                       cart.x() == this->x() &&
                       cart.y() == this->y();
            });

    if (it == std::end(carts))
        throw Ghost_crash{};

    it->crash();
    return it->get_curr();
}

bool operator<(const Cart& a, const Cart& b)
{
    return a.y() < b.y() ||
           (a.y() == b.y() && a.x() < b.x());
}

std::ostream& operator<<(std::ostream& os, const Cart& c)
{
    return os << char(c.facing()) << ' ' << c.x() << ',' << c.y();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

auto map_carts(const std::vector<std::string>& trax)
{
    std::vector<Cart> carts;
    const std::string cart_symbols {"^v<>"};

    for (size_t i = 0; i < trax.size(); ++i)
        for (size_t j = 0; j < trax[i].size(); ++j)
            if (cart_symbols.find(trax[i][j]) != std::string::npos)
                carts.emplace_back(Cart{int(j), int(i),
                                        Orientation(trax[i][j])});

    return carts;
}

bool advance_tick(std::vector<std::string>& trax, std::vector<Cart>& carts)
{
    bool any_crashes = false;

    std::sort(std::begin(carts), std::end(carts));

    for (auto& cart : carts) {

        char look_ahead = cart.look_ahead(trax);

        switch (look_ahead) {
        case '-':
        case '|':
            cart.advance(trax, look_ahead);
            break;
        case '/':
        case '\\':
            cart.advance(trax, look_ahead);
            cart.bend(look_ahead);
            break;
        case '+':
            cart.advance(trax, look_ahead);
            cart.turn();
            break;
        case '^':
        case 'v':
        case '<':
        case '>':
            cart.advance(trax, look_ahead);
            trax[cart.y()][cart.x()] = cart.crash(carts);
            any_crashes = true;
            break;
        case '\0':
            break;
        default:
            std::cerr << "Somehow got off the tracks with: "
                      << cart << '\n';
            throw Off_tracks{};
            break;
        }
    }

    if (any_crashes)
        carts.erase(std::remove_if(std::begin(carts), std::end(carts),
                [](auto& cart) {
                    return cart.facing() == Orientation::crash;
                }), std::end(carts));

    return any_crashes;
}

std::string find_crash(const std::vector<Cart>& carts)
{
    auto it = std::find_if(std::begin(carts), std::end(carts),
            [](const auto& cart) {
                return cart.facing() == Orientation::crash;
            });
    std::stringstream ss;
    ss << it->x() << ',' << it->y();
    return ss.str();
}

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 13 - Mine Cart Madness\n";

    auto trax = utils::get_input_lines(argc, argv, "13");
    auto cartz = map_carts(trax);
    /*
    while (!advance_tick(trax, cartz))
        ;
    auto part1 = find_crash(cartz);
    std::cout << "Part 1: " << part1 << '\n';
    */

    while (cartz.size() > 1) {
        //std::cout << "Number of carts: " << cartz.size() << '\n';
        advance_tick(trax, cartz);
    }
    auto part2 = cartz.front();
    std::cout << "Part 2: " << part2 << '\n';
}
