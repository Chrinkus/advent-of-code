#ifndef SUMMED_AREA_TABLE
#define SUMMED_AREA_TABLE

#include <Matrix11.h>

namespace utils {

using Numeric_lib::Matrix;

template<typename T>
class Summed_area_table {
public:
    Summed_area_table(long width, long height)
        : table{Matrix<T,2>(height + 1, width + 1)} { table = 0; }

    template<typename F>
    Summed_area_table& apply(F f)
    {
        for (long y = 1; y < table.dim1(); ++y)
            for (long x = 1; x < table.dim2(); ++x)
                table(x, y) = f(x, y);
    }

    long size() const { return table.size(); }
private:
    Matrix<T,2> table;
};

} // utils

#endif  // SUMMED_AREA_TABLE
