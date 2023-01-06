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

### Challenge Notes

Nice intro puzzle! One char moves us up, the other down. Where do we end up? Read the input like K&R taught us and switch on each paren. Part 2 asks for the position of the first character that sends us to the basement (below 0).

### Library Notes

No library usage today.

## Day 2 - I Was Told There Would Be No Math

This title is ridiculous considering the depth to which AoC challenges our understanding of math concepts.

### Challenge Notes

Juggling three values is always a tricky task. It seems like there should be a more elegant way to do it but I never find it. I opted to sort the three elements in part 1 and part 2 validated that decision. Initially I used three if-swaps but ditched that plan for using qsort because why not?

### Library Notes

No library usage today.

## Day 3 - Perfectly Spherical Houses in a Vacuum

No clue what this title is in reference to..

### Challenge Notes

A 2D puzzle on day 3! We need to move a virtual sleigh one 'space' at a time and log its location. Then for part 2 we need to alternate moving two sleighs. This challenge is a big reason why I came back to 2015. More on that below..

### Library Notes

Time to dust off my new infinite 2D space data structure in my fruity library! A hashed set of points let me log each location and then query the length for total houses. It's elegant and easy! So proud of how this worked out.

Big win for fruity!

My original solution had me implementing a binary search tree for points. Not a bad idea but definitely a lot of code to minimally write the needed structure. It also leaked memory as I allocated a BUNCH of points but cleaned up nary a one.

## Day 4 - The Ideal Stocking Stuffer

A joke about crypto currency! This is aging poorly..

### Challenge Notes

An MD5 hash challenge! You don't see these so much in the newer events. I believe it was never Eric's intent to have people implement their own MD5 hash implementation, reading the RFA makes your head spin!

This solution executes very slowly so I stopped converting my hashes to strings and just looked at the bytes. A byte equals two hex chars so I needed to isolate the first 5 and 6 chars for parts 1 and 2. An even number is easy, just divide by two. For part 1, with an odd number of zeros to find, I checked that the third byte was less than 16 to just examine the "upper" zeros.

This cut the execution time by a factor of 10! I guess `sprintf`-ing all those hex chars is expensive!

### Library Notes

For my first attempt at this problem I used the open-ssl library that was installed on my system. It was terribly slow but it worked. This time I needed something that CMake could build locally so I took to the internet to find one!

The RFA has a C implementation but I was not interested in copy-pasting that code myself. I found a repo on GitHub of someone who did though and forked it, added CMake build support, tweaked a few lines of code and had exactly what I needed!

UPDATE: I was still using `sprintf` to convert my int counter to a string for concatenation. Seeing how it slowed down the solution on the back-end I decided to implement my own `string_from_int` function. This shortened execution speed further!
