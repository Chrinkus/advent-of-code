#include <iostream>
#include <vector>
#include <list>
#include <numeric>

#include <get_input.hpp>

using Walker = std::vector<int>::const_iterator;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Node {
public:
    Node(int n_child, int n_metad)
        : num_child{n_child}, num_metadata{n_metad} { }

    int meta_sum() const;
    int get_value() const;

    void add_child(Node* p) { children.push_back(p); }
    void add_metadata(int md) { metadata.push_back(md); }

private:
    int num_child;
    int num_metadata;
    std::vector<Node*> children;
    std::vector<int> metadata;
};

int Node::meta_sum() const
{
    return std::accumulate(std::begin(metadata), std::end(metadata), 0);
}

int Node::get_value() const
{
    if (num_child == 0)
        return meta_sum();

    int sum = 0;
    for (const auto md : metadata)
        if (md <= num_child)
            sum += children[md-1]->get_value();     // md indexes are 1-based

    return sum;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Node* create_node(Walker& it, std::list<Node>& owner)
    // The iterator is never checked against end of sequence because the input
    // data will cancel itself.
{
    auto n_child = *it++;
    auto n_metad = *it++;

    owner.emplace_back(Node{n_child, n_metad});
    auto p_node = &owner.back();

    for (int i = 0; i < n_child; ++i)
        p_node->add_child(create_node(it, owner));      // recurse

    for (int i = 0; i < n_metad; ++i)
        p_node->add_metadata(*it++);

    return p_node;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Tree {
public:
    explicit Tree(const std::vector<int>& input);

    int get_meta_sum() const;
    int get_root_val() const;

private:
    Node* root;
    std::list<Node> owner;
    // Changed 'owner' to a std::list after a bunch of sneaky errors ended up
    // being caused by vector reallocations. Rather than tag each of the
    // children with a parent tag to call back to on a move I went with the
    // list.
    // Sneaky Errors: Seg-fault on large input set, incorrect metadata sums and
    // missing children on the smaller test input.
};

Tree::Tree(const std::vector<int>& input)
{
    auto it = std::begin(input);
    root = create_node(it, owner);
}

int Tree::get_meta_sum() const
{
    return std::accumulate(std::begin(owner), std::end(owner), 0,
            [](auto& sum, const auto& n) {
                return sum + n.meta_sum();
            });
}

int Tree::get_root_val() const
{
    return root->get_value();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 8 - Memory Maneuver\n";

    auto input = utils::get_input_values<int>(argc, argv, "08");
    auto tree = Tree{input};

    auto part1 = tree.get_meta_sum();
    std::cout << "Part 1: " << part1 << '\n';
    auto part2 = tree.get_root_val();
    std::cout << "Part 2: " << part2 << '\n';
}
