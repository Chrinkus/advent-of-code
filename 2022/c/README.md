# The C Journal

This year I will be utilizing my general-purpose libraries [libcgs](https://github.com/Chrinkus/libcgs/tree/v0.5) and [fruity](https://github.com/Chrinkus/fruity.git). CMake will take care of those dependencies when you run the following commands:

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

## Day 8 - Treetop Tree House

### Challenge Notes

This was a nice return to 2D map-based problems. I have two overly-long functions that process the views and scores in the four cardinal directions. They're straight-forward enough but contain a lot of almost repetitive code. I'm not sure of how to abstract the minor changes between up, down, left and right loops into a smaller format so they stay as written.

Most of the time was spent on library stuff..

### Library Notes

A 2D problem heralded the return of my Fruity library. I haven't used it in a while and spent a bit of time bringing the API up to my current standard of usability. There are a few changes I still plan to make but for now it was fun to get back into it.

My questionable decision making in designing early parts of libcgs came up again, this time with the `cgs_io_readlines` function. For whatever reason, when designing it I decided to try to store the lines as `char*`'s. I even acknowledged the problems associated with iterating through a vector of `*(char**)`'s but the best solution I found was to hide the hideousness behind typedef's.

Now its suddenly so clear that I should have just used `cgs_string`'s. This change exposed a bug in my `cgs_string_copy` code so the decision to change was a two-fer!

## Day 9 - Rope Bridge

### Challenge Notes

Aaarrrgghh.. an infinite grid problem. Definitely read the challenge and went to bed. I just don't yet have an elegant approach to solving these. Hash table of points? Set of points? I should probably try both and see what feels best. My gut tells me to use a set so the closes thing I have so far are my trees.

Also, points! For whatever reason I keep writing my point struct implementations in the solutions. I need to put them into my library but I never feel like I understand enough about the problem domain to do a good job abstracting a general point class.

Anyway, after much fussing I got part 1 done only to find that I had not made my code flexible enough to accept multi-knot ropes as input. The re-design was much quicker than I expected though and it worked almost immediately.

### Library Notes

The BIGGEST change today is that I figured out how to do dependency resolution with CMake and its so. darn. easy! Now anyone who wants to compile my code can do so much easier thanks to `FetchContent`.

I just mentioned not feeling confident enough in my domain knowledge to implement a general point class but that didn't seem to stop me when I wrote my binary-search tree. This thing is OLD and needs a facelift. I did some tweaks in place but I'll need to come back and give it some love later.

## Day 10 - Cathode-Ray Tube

### Challenge Notes

Time to write a graphics driver for our "handheld device". I was aware that the 'X' variable was starting on 1 but I got tripped up for a bit by the cycle count also starting on 1. Maybe it was clearer for others in the instructions but that got me. It didn't help that all of my "interesting signals" were accurate with the test data except for the last one.

Part 2 was classic AoC. Again, off by one was the name of the game though I just stared at the problem until it made sense to just subtract 1 from the cycles and leave X as is.

### Library Notes

I had some issues with my build system after the changes for the previous day. I thought I had it all figured out but my include path was messed up. I was still reading the include files that were installed on my machine as opposed to the ones in my build directory. The CMake discourse was quick and helpful for figuring out the fix!

I got to use fruity again but more as a display library. Again, it could be stream-lined a bit but for now the goal is catching up!
