#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Particle {
    Particle ()
        : pos{vector<int64_t>(0)}, vel{vector<int64_t>(0)},
        acc{vector<int64_t>(0)}, active{true} { }

    Particle (vector<int64_t>& p, vector<int64_t>& v, vector<int64_t>& a)
        : pos{p}, vel{v}, acc{a}, active{true} { }

    void tick(int);                 // update the particle
    int64_t dist() const;           // get Manhatten distance from origin
    void cancel() { active = false; };

    vector<int64_t> pos;
    vector<int64_t> vel;
    vector<int64_t> acc;
    bool active;
};

void Particle::tick(int n = 1)
{
    for (int i = 0; i < vel.size(); ++i) {
        if (active) {
            vel[i] += acc[i] * n;
            pos[i] += vel[i] * n;
        }
    }
}

int64_t Particle::dist() const
{
    int64_t sum = 0;
    if (active) {
        for (auto& a : pos)
            sum += abs(a);
    }
    return sum;
}

istream& operator>>(istream& is, Particle& p)
{
    vector<string> s (3);
    for (int i = 0; i < s.size(); ++i)
        is >> s[i];

    vector<int64_t> pp (3);
    vector<int64_t> vv (3);
    vector<int64_t> aa (3);
    for (auto& a : s) {
        istringstream iss {a};
        char ch1, ch2;              // ch2 is for burning chars
        int64_t i0, i1, i2;
        iss >> ch1 >> ch2 >> ch2 >> i0 >> ch2 >> i1 >> ch2 >> i2;

        switch(ch1) {
            case 'p': pp [0] = i0; pp [1] = i1; pp [2] = i2; break;
            case 'v': vv [0] = i0; vv [1] = i1; vv [2] = i2; break;
            case 'a': aa [0] = i0; aa [1] = i1; aa [2] = i2; break;
            default:
                throw runtime_error("Particle(): unknown char");
                break;
        }
    }

    Particle t {pp, vv, aa};
    p = t;

    return is;
}

ostream& operator<<(ostream& os, Particle& p)
{
    return os << "Particle: \n"
              << "  position ("
                    << p.pos[0] << ", " << p.pos[1] << ", " << p.pos[2] << ")\n"
              << "  velocity ("
                    << p.vel[0] << ", " << p.vel[1] << ", " << p.vel[2] << ")\n"
              << "  accel    (" 
                    << p.acc[0] << ", " << p.acc[1] << ", " << p.acc[2] << ")";
}

bool operator==(const Particle& a, const Particle& b)
{
    return a.pos[0] == b.pos[0] &&
           a.pos[1] == b.pos[1] &&
           a.pos[2] == b.pos[2];
}

vector<Particle> get_input(const string& s)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not read from file " + s);

    vector<Particle> v;
    for (Particle p; ifs >> p; )
        v.push_back(p);

    return v;
}

void mass_ticking(vector<Particle>& v, int n = 1)
{
    for (auto& a : v)
        a.tick(n);
}

int closest_to_origin(vector<Particle>& v)
{
    int64_t dist = INT64_MAX;
    int loc = 0;
    for (int i = 0; i < v.size(); ++i) {
        int64_t d = v[i].dist();
        if (d < dist) {
            dist = d;
            loc = i;
        }
    }
    return loc;
}

int closest_over_time(vector<Particle> v)
    // pass by value so as not to alter v
{
    mass_ticking(v, 500);
    return closest_to_origin(v);
}

vector<Particle*> detect_collisions(vector<Particle>& v)
{
    vector<Particle*> collisions;
    for (auto& a : v)
        if (a.active)
            for (auto& b : v)
                if (&a != &b && (a == b && b.active))
                    collisions.push_back(&a);

    return collisions;
}

void cancel_collisions(vector<Particle*>& pv)
{
    for (auto& p : pv)
        p->cancel();
}

int count_active(vector<Particle>& v)
{
    int count = 0;
    for (auto& a : v)
        if (a.active)
            ++count;
    return count;
}

int resolve_collisions(vector<Particle> vp)
    // pass by value so as not to modify vp
{
    for (int i = 0; i < 500; ++i) {
        mass_ticking(vp);
        vector<Particle*> vpp = detect_collisions(vp);
        cancel_collisions(vpp);
    }
    return count_active(vp);
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day20" + input + ".txt";

    vector<Particle> vp = get_input(iname);

    int part_1 = closest_over_time(vp);
    int part_2 = resolve_collisions(vp);

    cout << "The answer to part 1 is: " << part_1 << '\n';
    cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
