# 2015 C Journal

These solutions will be utilizing CMake to resolve dependencies and build the code. From this directory enter the following:

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

From there, run any solution providing the input as standard input. Here is an example of running day 1 with input from a folder in my documents directory:

```
$ ./1501 < ~/Documents/aoc/2015-01.txt
```

Your filesystem may be set up differently.

## Day 1 - Not Quite Lisp

The Lisp programming language is all about the parenthesis though apparently when you achieve Lisp enlightenment you no longer see them.

#### Challenge Notes

Nice intro puzzle! One char moves us up, the other down. Where do we end up? Read the input like K&R taught us and switch on each paren. Part 2 asks for the position of the first character that sends us to the basement (below 0).

#### Library Notes

No library usage today.
