#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>

class Bad_mode{};

class Op {
public:
    Op() = delete;
    Op(int64_t input);

    int code() const { return oc; }
    int mode(size_t i) const { return m[i]; }
private:
    int oc;
    std::array<int,3> m;
};

std::ostream& operator<<(std::ostream& os, const Op& op);

class Intcode_vm {
public:
	Intcode_vm() = delete;
	Intcode_vm(const std::string& program);

	~Intcode_vm() = default;

    int64_t zero() const { return vi[0]; }
    void zero(int64_t val) { vi[0] = val; }

	size_t size() const { return vi.size(); }
    bool halted() const { return h; }

    void run(std::istream& is = std::cin, std::ostream& os = std::cout);
    void config(int64_t noun, int64_t verb);

private:
    int64_t& get_param(int mode, size_t param);

private:
	std::vector<int64_t> vi;	// instructions
    ssize_t ip = 0;             // instruction pointer
    ssize_t rb = 0;             // relative base
    bool h = false;             // halted status
};
