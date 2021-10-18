#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char delim);

template<typename T, typename Mod>
std::vector<T> split(const std::string& s, char delim, Mod mod);
