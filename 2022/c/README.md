# The C Journal

This year I will be utilizing my general-purpose library [libcgs](https://github.com/Chrinkus/libcgs/tree/v0.5). With that dependency installed all that is needed to build these solutions is the following:

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

From the build directory, run any solution providing the input as `stdin`. Here is an example of running day 1 with input from a folder in my documents directory:

```
$ ./2201 < ~/Documents/aoc/2022-01.txt
```

Your filesystem setup may be different.

## Day 1 - Calorie Counting

The challenge here was the read. The input consists of groups of single integer lines separated by a blank line. We are to add up the integers in each group and record the sum. Which ever sum is the greatest is the answer to part 1. Part 2 just takes the sum of the top 3.

I needed to quickly add a reverse-integer-sort to my library for sorting from largest to smallest. Some further possible additions to the library are a max-element function and possibly a function to get the sum of a vector slice.

## Day 2 - Rock Paper Scissors

Wow! This puzzle exposed a quirk in my childhood, in our primary school we played "Scissors, paper, rock". Part 1 kept failing because I was reading 'A' as scissors, 'B' as paper and 'C' as rock. Not only was I processing the opponents moves wrong, I was all over the place on parsing the results.

The key here was converting the mapping the letter-based input to enums with sensible names to keep the logic straight when reading the code.

The reads were easy, `scanf` to the rescue once again!

Anyway, dual three-way comparisons are awkward so I look forward to sniping a better solution than `if`'s inside `switch`'s or dispatching to small functions.

## Day 3 - Rucksack Reorganization

### Library Notes

Every character-fiddling problem seems to expose a flaw in my string implementation. This time it was how to handle a vector of strings, or, how to clean up afterwards. I had to add a `vector_free_all_with` function to take a custom deallocator. After that it was all about char-counting.

In 2020 I made my first attempt at solving AoC in C using my first attempt at a general C library. That one had a lot of forbidden macro magic in it and I decided to start over with my current library. One bit that I miss was my char-counting struct. If another day comes up that uses it I'll strongly consider adding a char counter to my new library.

### Challenge Notes

Counting characters and detecting matches is fun in C, I'm not looking forward to doing this in Rust. Maybe I'll learn something.

Part 2 brought up another 3-way comparison situation. Interesting, seems to be a sub-theme this year.. I thought of sorting before comparing but in the end I started flipping zeroes in an array and checking counts for repeats. The 'a' = 1 instead of 0 initially seemed like it could be an annoyance but instead I extended the count buffer by one and ignored the first element.

## Day 4 - Camp Cleanup

This was a line collision problem. We're tasked to count when one line contains another and when two lines overlap in any way. Nice and short.

No library needed today! Though maybe I should have some 1D, 2D and 3D collision detection ready in my bag of tricks..

## Day 5 - Supply Stacks

### Challenge Notes

The READ! Another two-section input set. Then we're moving vertical stacks across lines! I usually don't like to make assumptions about my input but I had to hard-code the stride for the column reads. I ended up wrapping the stack vectors in structs. Not sure if that was best or not.

The moving of crates wasn't that bad initially but part 2 requires a ranged swap on a vector which I don't have yet. Maybe something to add later. In the meantime I used a third vector as a buffer.

### Library Notes

Added a few functions to the vector implementation. I've wanted a `_first` and `_last` for a while but didn't have a good idea for how to make that work for generic types. More and more I'm turning to `memcpy` to get these things done. I'm sure if I look it up I'll find a few articles about how DANGEROUS and HACKY memcpy is but whatever.

Another addition was a `_pop` function, also utilizing `memcpy` to great success. This was useful for part 1 with the one-at-a-time element moves. I'll need to get to the drawing board for a ranged swapper. My early thoughts is that I will shave a slice off the back of a source vector and place it on the back of a destination vector. But what to call it..?

## Day 6 - Tuning Trouble

### Challenge Notes

What a delightful challenge! I used a ring-y buffer for my last n-characters and it worked well. The test for different characters felt brute-force-ish but it still short circuits on failure so I feel it's fast enough.

### Library Notes

No libary usage today.

## Day 7 - No Space Left On Device

I have a feeling Mr Wastl has an Apple Watch..

### Challenge Notes

What an exciting problem set! Parsing Linux-like command line output and mapping a directory tree! This was super fun and the only problem I had was a lack of time due to a busy work and family week.

### Library Notes

I needed to write a `_strsub_to_int` function for int parsing. It was a fun challenge and I look forward to improving it to make it resilient to overflow. In general, the `strsub` implementation was very useful.

I write a lot of for-loops that I hoped to replace with my `vector_foreach` but they're all part of recursive functions and require more than a single 'user-data' member.
