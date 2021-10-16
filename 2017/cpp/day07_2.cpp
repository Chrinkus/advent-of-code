#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

struct Tower {
    Tower(const string& n, int w, vector<string>& c)
        : name{n}, weight{w}, child_names{c} { }

    Tower(const string& n, int w)
        : name{n}, weight{w} { }

    Tower()
        : name{""}, weight{0}, child_names{vector<string>{}} { }

    string name;
    int weight;
    vector<string> child_names;
};

istream& operator>>(istream& is, Tower& tt)
{
    string line;
    getline(is, line);
    istringstream iss {line};

    string nn;
    int ww;
    char ch1, ch2;
    iss >> nn >> ch1 >> ww >> ch2;

    string a;
    vector<string> v;
    if (iss >> a && a == "->") {
        for (string s; iss >> s; ) {
            if (s.back() == ',') s.pop_back();
            v.push_back(s);
        }
    } 

    tt = Tower{nn, ww, v};

    return is;
}

ostream& operator<<(ostream& os, Tower& tt)
{
    return os << tt.name << '\t'
              << tt.weight << '\t'
              << tt.child_names.size() << '\n';
}

void fill_from_file(istream& is, vector<Tower>& v)
    // exercise 1
{
    for (Tower t; is >> t; )
        v.push_back(t);
}

void fill_from_file(istream& is, map<string, Tower>& m)
    // exercise 2
{
    for (Tower t; is >> t; )
        m[t.name] = t;
}

Tower* filter_out_children(const vector<Tower*>& pv) {
    vector<Tower*> vt;

    for (const auto& a : pv) {
        bool match = false;

        for (const auto& b : a->child_names)
            for (const auto& c : pv)
                if (c->name == b)
                    match = true;

        if (match) vt.push_back(a);
    }
    if (vt.size() > 1)
        return filter_out_children(vt);
    else
        return vt[0];
}

Tower* get_root(vector<Tower>& v)
{
    vector<Tower*> pvt;
    for (auto& a : v)
        pvt.push_back(&a);

    return filter_out_children(pvt);
}

Tower* get_root(map<string, Tower>& m)
{
    vector<Tower*> pvt;
    for (auto& a : m)
        pvt.push_back(&a.second);

    return filter_out_children(pvt);
}

struct Node {
    Node(Tower* p, vector<Node> v)
        : tower{p}, children{v} { }

    Node(const Node& n)
        : tower{n.tower}, children{n.children} { }

    Node& operator=(const Node&);

    Tower* tower;
    vector<Node> children;
};

Node& Node::operator=(const Node& n)
    // make this Node a copy of n
    // is this necessary?
{
    tower = n.tower;
    children = n.children;
    return *this;
}

Node get_node(Tower* p, map<string, Tower>& m)
{
    vector<Node> vn;

    for (const auto& a : p->child_names) {
        Tower* t = &m[a];
        Node n = get_node(t, m);
        vn.push_back(n);
    }

    Node nn {p, vn};
    return nn;
}

int get_child_sums(Node& n)
{
    int sum = 0;
    for (auto& a : n.children)
        sum += a.tower->weight + get_child_sums(a);

    return sum;
}

vector<int> get_weight_dist(Node& n)
{
    vector<int> weights;
    for (auto& a : n.children) {
        int w = a.tower->weight + get_child_sums(a);
        weights.push_back(w);
    }
    return weights;
}

int find_diff(vector<int>& vi)
    // return the index where the different value is found or -1 if all the
    // same
{
    for (int i = 0; i < vi.size(); ++i) {
        if (vi[i] != vi.back()) {
            return vi[i+1] == vi.back() ? i : vi.size() - 1;
        }
    }
    return -1;
}

void track_it_down(Node& n)         // Error happens here at end of while loop
{
    Node* nn = &n;

    while(nn->children.size() > 0) {
        vector<int> weights = get_weight_dist(*nn);

        cout << nn->tower->name << '\n';
        for (auto& a : weights)
            cout << a << '\t';
        cout << '\n';
        for (auto& a : nn->children)
            cout << a.tower->name << '\t' << a.tower->weight << '\n';

        int diff = find_diff(weights);

        if (diff == -1) return;
        nn = &nn->children[diff];         // crashes here
    }
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "./day7_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    map<string, Tower> ut;
    fill_from_file(ifs, ut);

    Tower* pr = get_root(ut);
    Node tree = get_node(pr, ut);

    track_it_down(tree);
    /*
    vector<int> test_w = get_weight_dist(tree);
    for (const auto& a : test_w)
        cout << a << '\n';
        */
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
