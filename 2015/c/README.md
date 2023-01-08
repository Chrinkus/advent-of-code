# 2015 C Journal

Each day I will try to comment on three aspects of my approach:

1. **Challenge Notes** - How I approached the challenge.
2. **Now vs Then** - I first attempted these problems two years ago when I was new to C. The code I wrote then has been moved to the `archive/` directory to track my progress.
3. **Library Notes** - Specific parts of my support libraries that were used to complete the challenge. I'll make special note of times where changes to a support library were required to provide the appropriate functionality.

## Building and Running Solutions

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

## Day 5 - Doesn't He Have Intern-Elves For This?

This title is the first hint that we are, as AoC participants, intern-elves for Santa.

### Challenge Notes

String validation. I'm not wild about these problems but its fun trying to read each string as few times as possible. Single-pass solutions, short-circuits to skip unnecessary reads.

That's the fun stuff.

For my money, part 2 is easier than part 1 in that both "conditions" can be checked with a 2nd iterator two characters along in the string.

### Now vs Then

It is EERIE how my switch statments are IDENTICAL to my thought process two years ago! The first condition in part 1 is kind of automatic but condition 3, right down to the TAB-ing is just my style, I guess.

As for the differences, again I'm using integers for booleans instead of including `stdbool`. The pointer use is much more confident. Also I write a very hectic dynamic string reader for grabbing the input. Just reading it I can't be certain that it works correctly.

### Library Notes

Just the dynamic string and my getline functions here. Not a lot of library use but at least I didn't have to implement them in the solution file!

## Day 6 - Probably a Fire Hazard

### Challenge Notes

Operations on ranges of 2D data. The bounds of the grids are known at compile time so I just used the regular C-arrays. Part 1 only needs bits so I used `int8_t`s. Chars also worked but I felt that a numeric type was a better fit for the purpose.

The hardest part was the read. Initially I was trying to just `scanf` from input but since the input lines all started with 't' a failing call would still consume the first letter. I could get by this by having the first pattern be "toggle.." and the second be "urn.." but that was kind of janky. In the end I grabbed the lines one at a time and `sscanf`d them with full patterns for clarity.

### Now vs Then

I was experimenting with approaches my first time through this problem. For part 1 I used a 2D C-array and for part 2 I tried a flattened 1D array with [row * i + col] indexing. I appreciate the desire to try them both. Good job, past me!

Again I wrote a custom `readline` function and even expressed the desire to "lib it out" in the comments.

Rather than doing a test and dispatching an appropriate function, I just parsed the line and passed all the data to another function to dispatch the different commands. It's kind of a mess of `enum`s and `switch` statements.

### Library Notes

Again, the only library usage was my `io_getline` function to simplify the reads. But that's the whole point, use a library to take care of the parts that aren't the immediate problem under consideration.

Though it is a 2D challenge, I decided not to use fruity as the boundaries were known at compile time so a regular 2D C-array was sufficiently easy to work with.

## Day 7 - Some Assembly Required

You knew a toy would be cool when it had this on the box.

### Challenge Notes

Whuf! Simulating a bunch of wire connections with different types of logic gates. This kind of problem would be much easier in an OO language but we can make it happen with C! This challenge represents the first star that I'm picking up on return to 2015.

Each line is the description of a wire, specifically the last word in the string. I was going to split the line but ended up writing a `scanf` for each 'type' of gate. After running into a wall due to some signals being passed as wires and others unexpectedly as raw integers, I decided to defer the detection of a number till later in the `get_signal` phase.

Part 2 was easy, IF you engineered your part 1 to be reset and re-run. Or if your support library already had a way to iterate over the elements of a hash table.

### Now vs Then

In my first attempt at this I also identified a hash table as the ideal structure for the problem. Then I tried to implement it.. As a result my solution for part 1 was 321 lines of code!

A mistake I made was to throw each line into a queue and process them only when there was a signal available for each parameter wire. I considered this possibility this time as well but figured that connencting the virtual wires then recursing up the line to valid signals would be better.

In the end I did not complete part 2 in 2020.

### Library Notes

So here's a funny thing. I implemented a hash table in `libcgs` a while ago but never ended up using it in the wild. This challenge is the first road test for my data structure and, while it got the job done, I will be making adjustments.

Part 1 was engineered in such a way that set up part 2 well. The only thing I needed was an iterator for the elements of a hash table. This was implemented successfully but I'll not consider the functionality finished until I review the whole hash table at a later date.
