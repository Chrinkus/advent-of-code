#include <iostream>
#include <vector>

std::string get_input_file(int argc, char** argv, const std::string& day)
{
    std::vector<std::string> varg { argv, argv + argc };

    auto it = ++std::begin(varg);
    while (it != std::end(varg)) {
        if (*it[0] == '-')
            switch (*it[1]) {
            case 't':
                // its a test
                break;
            case 'f':
                // its another file..?
                break;
            default:
                // Unknown flag
                break;
            }
        else
            ++it;
    }
}

int main(int argc, char* argv[])
{
    auto fname = get_input_file(argc, argv, "07");
}
