# advent-of-code-2018

This repository stores my solutions to the daily challenges offered by the amazing [Advent of Code](https://adventofcode.com) event hosted every December by [Eric Wastl](https://twitter.com/ericwastl?lang=en-ca).

I have recently implemented CMake support to simplify compiling and linking. The steps for building are as follows (assuming you have [CMake](https://cmake.org) installed on your system).

From the project root:
```
$ mkdir build && cd build
$ cmake ..
$ make
$ ./src/day01 [-t]
```

The last line shows how to build the solution for day 1. The `build/src/` subdirectory will hold all of the available solutions. The standard input will be used if called as above. Enter the test flag `-t` to run the solution with test input.

Alternate solutions (or horrendously hackey solutions) will be saved in the `scratch/` folder for archival purposes. These will not be built automatically and are not even guaranteed to compile. I'm considering renaming the folder to `boneyard/`.

### Bjarne Stroustrup's Matrix Library

I have added the Matrix library that is used in §24 of _Programming Principles and Practice Using C++_ by Bjarne Stroustrup. It will be part of the repository and stored in the `/vendor/` directory. Any utilities that I write to use this library will find a home in my `/include/` directory.

After several challenges necessitated the use of two-dimensional array/vectors I started thinking about how I would implement my own 2-dimensional grid data-structure. Realizing that I too often attempt to "roll my own", I figured working with a library was its own skill that I could work on.

### Update 2021-10-15

These solutions are being kept as-is, though it would be good to come back one day and FINISH them.. Anyway, this repo has been consolidated with my main advent-of-code repository.

