#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <sstream>
#include <cctype>       // isdigit, isxdigit
#include "Read_input.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Field definitions and validation
constexpr std::array<std::string_view,7> field_names {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
};

constexpr std::array<std::string_view,7> eye_colors {
    "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
};

enum class Field { byr, iyr, eyr, hgt, hcl, ecl, pid, cid };

Field get_field(const std::string& s)
{
    auto it = std::find(std::begin(field_names), std::end(field_names),
            s.substr(0, 3));
    if (it == std::end(field_names))
        return Field::cid;
    else
        return static_cast<Field>(std::distance(std::begin(field_names), it));
}

bool check_birth_year(const std::string& data)
{
    if (data.size() != 4)
        return false;
    auto y = std::stoi(data);
    return (1920 <= y && y <= 2020);
}

bool check_issue_year(const std::string& data)
{
    if (data.size() != 4)
        return false;
    auto y = std::stoi(data);
    return (2010 <= y && y <= 2020);
}
bool check_expire_year(const std::string& data)
{
    if (data.size() != 4)
        return false;
    auto y = std::stoi(data);
    return (2020 <= y && y <= 2030);
}
bool check_height(const std::string& data)
{
    std::istringstream iss {data};
    int height;
    std::string sys;
    iss >> height >> sys;
    if (sys == "cm")
        return (150 <= height && height <= 193);
    else if (sys == "in")
        return (59 <= height && height <= 76);
    else
        return false;
}
bool check_hair_color(const std::string& data)
{
    if (data.size() != 7 || data[0] != '#')
        return false;
    for (std::size_t i = 1; i != data.size(); ++i)
        if (!isxdigit(data[i]))
            return false;
    return true;
}
bool check_eye_color(const std::string& data)
{
    auto it = std::find(std::begin(eye_colors),
            std::end(eye_colors), data);
    return it != std::end(eye_colors);
}
bool check_passport_id(const std::string& data)
{
    if (data.size() != 9)
        return false;
    for (const auto ch : data)
        if (!isdigit(ch))
            return false;
    return true;
}

bool validate_entry(const std::string& entry)
{
    Field f = get_field(entry.substr(0, 3));
    std::string val = entry.substr(4);

    switch (f) {
        case Field::byr:    return check_birth_year(val);
        case Field::iyr:    return check_issue_year(val);
        case Field::eyr:    return check_expire_year(val);
        case Field::hgt:    return check_height(val);
        case Field::hcl:    return check_hair_color(val);
        case Field::ecl:    return check_eye_color(val);
        case Field::pid:    return check_passport_id(val);
        case Field::cid:    return true;
    }
    return false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Passport class
class Passport {
public:
    Passport() = default;
    explicit Passport(const std::string& s);

    bool has_valid_fields() const { return valid_fields; }
    bool is_valid() const { return valid_fields && valid_data; }

private:
    std::string data;
    bool valid_fields = false;
    bool valid_data   = false;

    void check_fields();
    void check_data();
};

Passport::Passport(const std::string& s)
    : data{s}
{
    check_fields();
    check_data();
}

void Passport::check_fields()
{
    valid_fields = std::all_of(std::begin(field_names), std::end(field_names),
            [this](const auto& f) {
                return this->data.find(f) != std::string::npos;
            });
}

void Passport::check_data()
{
    std::vector<std::string> entries;
    std::istringstream iss {data};
    for (std::string entry; std::getline(iss, entry, ' '); )
        entries.push_back(entry);
    
    valid_data = std::all_of(std::begin(entries), std::end(entries),
            validate_entry);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Input processing and main()
auto combine_passport_data(const std::vector<std::string>& input)
{
    std::vector<std::string> vs { "" };
    for (const auto& s : input) {
        if (s.empty()) {
            vs.push_back(s);
        } else {
            vs.back() += s + ' ';
        }
    }
    return vs;
}

std::pair<int,int> eval_passports(const std::vector<std::string>& vs)
{
    int count1 = 0, count2 = 0;
    for (const auto& s : vs) {
        Passport p {s};
        count1 += p.has_valid_fields();
        count2 += p.is_valid();
    }
    return std::make_pair(count1, count2);
}

int main()
{
    const auto input = read_input_lines();
    const auto raw_p = combine_passport_data(input);

    const auto [ part1, part2 ] = eval_passports(raw_p);
    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}