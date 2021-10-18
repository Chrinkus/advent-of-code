#pragma once

#include <string>
#include <iostream>

class Point {
public:
	Point() = default;
	Point(int x, int y) : xx{x}, yy{y} { }

	int x() const noexcept { return xx; }
	int y() const noexcept { return yy; }
	int manhatten_distance(const Point& p = Point{0, 0}) const noexcept;
    std::string to_string() const;

	void move(int dx, int dy) { xx += dx; yy += dy; }
private:
	int xx = 0;
	int yy = 0;
};

bool operator==(const Point& a, const Point& b) noexcept;
bool operator!=(const Point& a, const Point& b) noexcept;
bool operator<(const Point& a, const Point& b) noexcept;

std::ostream& operator<<(std::ostream& os, const Point& p);
