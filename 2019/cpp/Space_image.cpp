#include "Space_image.hpp"

bool is_valid_dimensions(const int w, const int h, const std::string& s)
{
    return s.size() % (w * h) == 0;
}

Space_image::Space_image(const int ww, const int hh, const std::string& raw_d)
    : w{ww}, h{hh}
{
    if (!is_valid_dimensions(ww, hh, raw_d))
        throw Bad_image_data{};

    for (auto i = 0; i < h; ++i)
        d.push_back(raw_d.substr(i * w, w));
}
