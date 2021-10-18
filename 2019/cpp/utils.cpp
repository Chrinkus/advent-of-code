#include "utils.hpp"

#include <sstream>

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> vs;
	std::istringstream iss {s};
	for (std::string sub; std::getline(iss, sub, delim); )
		vs.push_back(sub);
    return vs;
}

template<typename T, typename Mod>
std::vector<T> split(const std::string& s, char delim, Mod mod)
{
	std::vector<T> vt;
	std::istringstream iss {s};
	for (std::string sub; std::getline(iss, sub, delim); )
		vt.push_back(mod(sub));
	return vt;
}

