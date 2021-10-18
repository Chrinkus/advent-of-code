#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Layer {
    const std::string data;
    const ssize_t zeros;
    Layer(std::string s, ssize_t z) : data{std::move(s)}, zeros{z} { }
};

bool cmp_zeros(const Layer& a, const Layer& b)
{
    return a.zeros < b.zeros;
}

int main()
{
    std::cout << "Advent of Code 2019 - Day 8\n"
              << "Space Image Format\n";

    std::string input;
    std::cin >> input;

    constexpr int width = 25;
    constexpr int height = 6;
    constexpr int layer_sz = width * height;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

    std::vector<Layer> vl;
    for (auto i = 0; i < input.size(); i += layer_sz) {
        auto s = input.substr(i, layer_sz);
        auto n = std::count(std::begin(s), std::end(s), '0');
        vl.emplace_back(Layer{s, n});
    }

    auto s = std::min_element(std::begin(vl), std::end(vl), cmp_zeros)->data;

    /* Two passes
    int ones = std::count(std::begin(s), std::end(s), '1');
    int twos = std::count(std::begin(s), std::end(s), '2');
    */
    // One pass
    auto ones = 0;
    auto twos = 0;
    for (const auto c : s)
        switch (c) {
            case '1':   ++ones; break;
            case '2':   ++twos; break;
        }

	auto part1 = ones * twos;
	std::cout << "Part 1: " << part1 << '\n';

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

    std::string raw_image;
    for (auto i = 0; i < layer_sz; ++i) {
        for (const auto& l : vl)
            if (l.data[i] == '1') {
                raw_image += '*';
                break;
            } else if (l.data[i] == '0') {
                raw_image += ' ';
                break;
            }
    }
    std::vector<std::string> image;
    for (auto i = 0; i < height; ++i)
        image.push_back(raw_image.substr(i * width, width));

    for (const auto& s : image)
        std::cout << s << '\n';
}
