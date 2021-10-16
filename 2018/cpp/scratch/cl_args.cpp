#include <iostream>

#include <get_input.hpp>

int main(int argc, char* argv[])
{
    auto input = utils::get_input_string(argc, argv, "09");
    std::cout << input << '\n';
}
