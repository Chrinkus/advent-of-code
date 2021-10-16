#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <numeric>

#include <get_input.hpp>

const int int_max = std::numeric_limits<int>::max();

class Unknown_char{};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Point {
private:
    int xx, yy;
public:
    Point(int x, int y) : xx{x}, yy{y} { }

    int x() const { return xx; }
    int y() const { return yy; }

    const std::vector<Point> get_adjacents() const;
    const std::vector<Point> get_diagonals() const;

    void move(int dx, int dy) { xx += dx; yy += dy; }
};

const std::vector<Point> Point::get_adjacents() const
{
    std::vector<Point> vadj;
    vadj.emplace_back(Point{x(), y()-1});
    vadj.emplace_back(Point{x()-1, y()});
    vadj.emplace_back(Point{x()+1, y()});
    vadj.emplace_back(Point{x(), y()+1});
    return vadj;
}

const std::vector<Point> Point::get_diagonals() const
{
    std::vector<Point> vdia;
    vdia.emplace_back(Point{x()-1, y()-1});
    vdia.emplace_back(Point{x()+1, y()-1});
    vdia.emplace_back(Point{x()-1, y()+1});
    vdia.emplace_back(Point{x()+1, y()+1});
    return vdia;
}

const Point create_projection(const Point& a, const Point& b)
{
    return Point{ 2 * b.x() - a.x(), 2 * b.y() - a.y() };
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

template<typename T>
class Grid {
private:
    std::vector<std::vector<T>> grid;
    const T filler;
public:
    Grid(size_t width, size_t height, T fill_value);

    T& operator()(const Point& p) { return grid[p.y()][p.x()]; }
    T& operator()(size_t x, size_t y) { return grid[y][x]; }

    void print() const;
    void check_size(Point& p);
    void change_value(const T from, const T to);

    const Point get_centre() const;
    const std::vector<std::vector<T>>& get_grid() const { return grid; }

    size_t width() const { return grid.front().size(); }
    size_t height() const { return grid.size(); }
};

template<typename T>
Grid<T>::Grid(size_t width, size_t height, T fill_value)
    : grid{std::vector<std::vector<T>>(height)}, filler{fill_value}
{
    for (auto& vt : grid) {
        vt.resize(width);
        std::fill(std::begin(vt), std::end(vt), filler);
    }
}

template<typename T>
void Grid<T>::print() const
{
    for (const auto& vt : grid) {
        for (const auto t : vt)
            std::cout << t;
        std::cout << '\n';
    }
}

template<typename T>
void Grid<T>::check_size(Point& p)
    // potentially long reallocation so increasing size by 8 to allow for
    // a few increases in any given direction before another realloctaion
{
    const int size_inc = 8;
    
    if (p.y() - 3 < 0) {
        std::vector<char> row (grid.front().size());
        std::fill(std::begin(row), std::end(row), filler);
        grid.insert(std::begin(grid), size_inc, row);
        p.move(0, size_inc);
    } else if (p.y() + 3 >= int(grid.size())) {
        std::vector<char> row (grid.front().size());
        std::fill(std::begin(row), std::end(row), filler);
        grid.insert(std::end(grid), size_inc, row);
    }

    if (p.x() - 3 < 0) {
        for (auto& row : grid)
            row.insert(std::begin(row), size_inc, filler);
        p.move(size_inc, 0);
    } else if (p.x() + 3 >= int(grid.front().size())) {
        for (auto& row : grid)
            row.insert(std::end(row), size_inc, filler);
    }
}

template<typename T>
void Grid<T>::change_value(const T from, const T to)
{
    for (auto& row : grid)
        for (auto& t : row)
            if (t == from)
                t = to;
}

template<typename T>
inline const Point Grid<T>::get_centre() const
{
    return Point{int(grid.size() / 2), int(grid.front().size() / 2)};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Facility {
private:
    Grid<char> fmap;
    Grid<int>  dmap;
public:
    explicit Facility(const std::string& input, size_t shint = 1000);

    void print_map() const { fmap.print(); }

    int max_doors_from_X() const;
    int count_at_least_1000() const;

private:
    void create_room(const Point& p, const char mid = '.');
    const Point add_room(Point& p, char dir);

    void parse_map(const Point& start, const std::string& input);
    void map_rooms(Point p, std::string::const_iterator& it);
    void map_distances();
    void map_adjacents(const Point& p, int n);

    void write_adjacents(const Point& p, const char ch = '?');
    void write_diagonals(const Point& p, const char ch = '#');
    void write_questions();
};

Facility::Facility(const std::string& input, size_t shint)
    : fmap{Grid<char>{shint, shint, ' '}}, dmap{Grid<int>{shint, shint, -1}}
{
    auto centre = fmap.get_centre();
    create_room(centre, 'X');

    parse_map(centre, input);

    write_questions();

    map_distances();
}

int Facility::max_doors_from_X() const
{
    const auto& grid = dmap.get_grid();

    std::vector<int> max_per_row;
    for (const auto& row : grid) 
        max_per_row.push_back(*std::max_element(std::begin(row),
                                                std::end(row)));

    return *std::max_element(std::begin(max_per_row), std::end(max_per_row));
}

int Facility::count_at_least_1000() const
{
    const auto& grid = dmap.get_grid();

    std::vector<int> v1000;
    for (const auto& row : grid)
        v1000.push_back(std::count_if(std::begin(row), std::end(row),
                    [](const auto v){ return v >= 1000; }));

    return std::accumulate(std::begin(v1000), std::end(v1000), 0);
}

void Facility::map_distances()
{
    Point start {0, 0};

    for (size_t i = 0; i < fmap.height(); ++i)
        for (size_t j = 0; j < fmap.width(); ++j)
            switch (fmap(j, i)) {
            case 'X':
                start.move(j, i);
                dmap(j,i) = 0;
                break;
            case '-':
            case '|':
                dmap(j,i) = 0;
                break;
            case '.':
                dmap(j, i) = int_max;
                break;
            case '#':
            case ' ':
                dmap(j, i) = -1;
                break;
            default:
                std::cerr << "Unknown character in grid\n";
                throw Unknown_char{};
            }

    map_adjacents(start, 1);
}

void Facility::map_adjacents(const Point& p, int n)
{
    auto vadj = p.get_adjacents();

    for (const auto& adj : vadj)
        if (dmap(adj) == 0) {
            auto next = create_projection(p, adj);
            if (dmap(next) > n) {
                dmap(next) = n;
                map_adjacents(next, n + 1);
            }
        }
}

void Facility::create_room(const Point& p, const char mid)
{
    write_adjacents(p);
    write_diagonals(p);
    fmap(p) = mid;                
}

const Point Facility::add_room(Point& p, char dir)
{
    fmap.check_size(p);

    auto centre = p;
    switch (dir) {
    case 'N':
        fmap(Point{p.x(), p.y()-1}) = '-';
        centre.move(0, -2);
        break;
    case 'E':
        fmap(Point{p.x()+1, p.y()}) = '|';
        centre.move(2, 0);
        break;
    case 'W':
        fmap(Point{p.x()-1, p.y()}) = '|';
        centre.move(-2, 0);
        break;
    case 'S':
        fmap(Point{p.x(), p.y()+1}) = '-';
        centre.move(0, 2);
        break;
    default:    break;
    }

    create_room(centre);
    return centre;
}

void Facility::parse_map(const Point& start, const std::string& input)
{
    auto it = std::begin(input);

    map_rooms(start, ++it);             // ++ to skip opening '^'
}

void Facility::map_rooms(Point p, std::string::const_iterator& it)
{
    switch (*it) {
    case 'N':
    case 'E':
    case 'W':
    case 'S':
        p = add_room(p, *it);
        return map_rooms(p, ++it);
    case '(':
        //map_rooms(p, ++it);
        while (*it != ')')
            map_rooms(p, ++it);
        map_rooms(p, ++it);
    case '|':
        return;
    case ')':
        return;// map_rooms(p, ++it);
    case '$':
        ++it;
        return;
    default:
        break;
    }
}

void Facility::write_adjacents(const Point& p, const char ch)
{
    auto vadj = p.get_adjacents();

    for (auto& padj : vadj)
        if (fmap(padj) == ' ')
            fmap(padj) = ch;
}

void Facility::write_diagonals(const Point& p, const char ch)
{
    auto vdia = p.get_diagonals();

    for (auto& pdia : vdia)
        if (fmap(pdia) == ' ')
            fmap(pdia) = ch;
}

void Facility::write_questions()
{
    fmap.change_value('?', '#');
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main(int argc, char* argv[])
{
    std::cout << "AoC 2018 Day 20 - A Regular Map\n";

    auto input = utils::get_input_string(argc, argv, "20");
    auto facility = std::make_unique<Facility>(input, 1000);
    //facility->print_map();

    auto part1 = facility->max_doors_from_X();
    std::cout << "Part 1: " << part1 << '\n';

    auto part2 = facility->count_at_least_1000();
    std::cout << "Part 2: " << part2 << '\n';
}

