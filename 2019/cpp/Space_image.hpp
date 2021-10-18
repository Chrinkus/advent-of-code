#pragma once

#include <string>
#include <vector>
#include <iostream>

class Bad_image_data{};

class Space_image {
public:
    Space_image() = default;
    Space_image(const int ww, const int hh, const std::string& raw_d);

    int width() const { return w; }
    int height() const { return h; }

private:
    int w = 0;                      // image width
    int h = 0;                      // image height
    std::vector<std::string> d;     // image data
};

bool is_valid_dimensions(const int w, const int h, const std::string& s);
