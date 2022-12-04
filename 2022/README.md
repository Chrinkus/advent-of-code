# Advent of Code 2022

Another year, another wonderful event! This year I will be solving problems in C and Rust at the start. A significant change over previous years is that I will not be attempting the solutions as they release. Rather I will be valuing my sleep and attempting the problems the following day on my lunch break.

Something else I'd like to do is keep a running journal of my thought process each day. Returning to problems in previous years I have found that my current thinking is different at some points and I would like to have a more literal record of my process than just the code.

## Journal

### Day 1 - Calorie Counting

The challenge here was the read. The input consists of groups of single integer lines separated by a blank line. We are to add up the integers in each group and record the sum. Which ever sum is the greatest is the answer to part 1. Part 2 just takes the sum of the top 3.

I needed to quickly add a reverse-integer-sort to my library for sorting from largest to smallest. Some further possible additions to the library are a max-element function and possibly a function to get the sum of a vector slice.

### Day 2 - Rock Paper Scissors

Wow! This puzzle exposed a quirk in my childhood, in our primary school we played "Scissors, paper, rock". Part 1 kept failing because I was reading 'A' as scissors, 'B' as paper and 'C' as rock. Not only was I processing the opponents moves wrong, I was all over the place on parsing the results.

The key here was converting the mapping the letter-based input to enums with sensible names to keep the logic straight when reading the code.

The reads were easy, `scanf` to the rescue once again!

Anyway, dual three-way comparisons are awkward so I look forward to sniping a better solution than `if`'s inside `switch`'s or dispatching to small functions.

### Day 3 - Rucksack Reorganization

#### Library Notes

Every character-fiddling problem seems to expose a flaw in my string implementation. This time it was how to handle a vector of strings, or, how to clean up afterwards. I had to add a `vector_free_all_with` function to take a custom deallocator. After that it was all about char-counting.

In 2020 I made my first attempt at solving AoC in C using my first attempt at a general C library. That one had a lot of forbidden macro magic in it and I decided to start over with my current library. One bit that I miss was my char-counting struct. If another day comes up that uses it I'll strongly consider adding a char counter to my new library.

#### Challenge Notes

Counting characters and detecting matches is fun in C, I'm not looking forward to doing this in Rust. Maybe I'll learn something.

Part 2 brought up another 3-way comparison situation. Interesting, seems to be a sub-theme this year.. I thought of sorting before comparing but in the end I started flipping zeroes in an array and checking counts for repeats. The 'a' = 1 instead of 0 initially seemed like it could be an annoyance but instead I extended the count buffer by one and ignored the first element.
