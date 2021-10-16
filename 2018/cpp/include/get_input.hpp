#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace utils {

auto get_fname(int argc, char* argv[], const std::string& day)
{
    if (argc == 1)
        return "../inputs/input" + day + ".txt";

    std::vector<std::string> vargs {argv, argv + argc};

    if (argc == 2 && vargs[1] == "-t")
        return "../inputs/test-input" + day + ".txt";

    return vargs[1];
}

template<typename ...Args>
auto get_input_string(const Args& ...args)
{
    std::string fname = get_fname(args...);
    std::ifstream ifs {fname};
    if (!ifs)
        std::cerr << "Error: could not read from file: " << fname << '\n';

    std::string contents;
    ifs.seekg(0, std::ios::end);
    contents.resize(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ifs.read(&contents[0], contents.size());
    return contents;
}

template<typename ...Args>
auto get_input_lines(const Args& ...args)
{
    std::string fname = get_fname(args...);
    std::ifstream ifs {fname};
    if (!ifs)
        std::cerr << "Error: could not read from file: " << fname << '\n';

    std::vector<std::string> vs;
    std::string line;
    while (std::getline(ifs, line))
        vs.push_back(std::move(line));

    return vs;
}

template<typename T, typename ...Args>
auto get_input_value(const Args& ...args)
{
    std::string fname = get_fname(args...);
    std::ifstream ifs {fname};
    if (!ifs)
        std::cerr << "Error: could not read from file: " << fname << '\n';

    T t;
    ifs >> t;
    return t;
}

template<typename T, typename ...Args>
auto get_input_values(const Args& ...args)
{
    std::string fname = get_fname(args...);
    std::ifstream ifs {fname};
    if (!ifs)
        std::cerr << "Error: could not read from file: " << fname << '\n';

    std::vector<T> vt;
    for (T val; ifs >> val; )
        vt.push_back(val);

    return vt;
}

template<typename T, typename ...Args>
auto get_input_value_matrix(const Args& ...args, size_t s_hint = 100)
{
    std::string fname = get_fname(args...);
    std::ifstream ifs {fname};
    if (!ifs)
        std::cerr << "Error: could not read from file: " << fname << '\n';

    std::vector<std::vector<T>> vvt;
    vvt.reserve(s_hint);                // reallocations could get heavy

    for (std::string line; std::getline(ifs, line); ) {
        std::vector<T> vt;

        std::istringstream iss {line};
        for (T val; iss >> val; )
            vt.push_back(val);

        vvt.push_back(vt);
    }
    return vvt;
}

}   // namespace utils
