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

UPDATE: This challenge informed some changes that were made to `libcgs` to improve the hash table usage. These changes broke the API a little so this problem has been tweaked.

## Day 8 - Matchsticks

No clue what this title is in reference to aside from the appearance of multiple back-slashes in a row when escaping escape characters.

### Challenge Notes

Not gonna lie, this challenge turned me off a bit when I read it. Initially I believed I would have to create the new strings and such. As it turns out, just keeping counts going was a much easier solution. Part 2 fit right beside my part 1 counters and required minimal extra code.

### Now vs Then

Looks like I had the same idea back in 2020. I created a "lengths" struct and used it to keep track of extra characters. The execution was a little different, I added the full length of the source string to each type of count and performed both addition to encoding and subtraction from memory.

One thing of note is my standard library usage. I call `getline` from the standard library which was still a GNU extension in C99, which is the version of C that I was compiling originally. I'm sure I would have had an issue with this on Mac or Windows.

Further, I have a potential buffer overflow with my hard-coding of a 64-byte string for my line reads. Oops!

### Library Notes

Just a straight up `cgs_string` and `cgs_io_getline` day. Love those functions!

## Day 9 - All in a Single Night

\<Waves hands\> MAGIC!!

### Challenge Notes

Here we go, a graph problem. Specifically an all-points shortest path problem. I had to do some serious reading to get a quality solution together here. Cormen et al to the rescue!

Needing to visit all points had me performing Dijkstra's algorithm on each starting element and keeping a bit mask of the visited indexes. This took a bit to wrap my brain around but when I got it figured out it ran quick and gave a correct answer!

Part 2 asked for a longest path so I sought to parameterize my comparison functions to reuse as much of the first example as possible. I had to spot-write some functions that should have been available in my library, this will be commented on further down.

UPDATE: OMG THE READ! I had written the read when first approaching the problem again and then spent a week reading up on graph algorithms. I forgot how hefty the read was! It was not my initial plan but the idea of putting the distances into a matrix of paired weights made executing the pathfinding SO EASY. I got the idea from a [tweet](https://twitter.com/TivadarDanka/status/1612415042766663680?s=20).

### Now vs Then

There is no "then"! I didn't even attempt this problem in 2020 so here we are, striking new ground!

### Library Notes

I need to write a `cgs_vector_min` and corresponding `_max` function. I've needed them before but never wrote them. In this solution I use a `_foreach` so, 'yay' for library use but 'boo' for not having the best possible operation.

Heaps are working out REALLY well. I may need one more path-finding problem to have a better understanding of which parts of the solutions are transferrable and which parts should be parameterized.

UPDATE: The missing `min` and `max` were added!

## Day 10 - Elves Look, Elves Say

### Challenge Notes

A game of life puzzle. For these I like to just run the simulation and if it starts to take too long I'll look for  a pattern to short-circuit the later iterations. The "brute force" solution runs in a few milli-seconds so I'm happy with that.

Looking at the results the only thing I can pick out is that the values are all 1-3.

### Now vs Then

My initial approach was very similar to this most recent one. How I handle the growing "look-say" string is notable. I seem to just be re-allocating to 1.5 times the previous length every iteration. It's an arbitrary scale factor that must have *just worked*.

Another thing I do is have a 'length' counter that I'm adding to rather than just reading the length of the resulting string.

### Library Notes

My `string` functions really shine here. I'm happy to have `push`, `clear`, and `move` implemented for use. There's a `SWAP` sighting as well!

## Day 11 - Corporate Policy

I love the "passwords must be exactly eight lowercase letters (for security reasons)" line.

### Challenge Notes

Password validation! We have three requirements to meet, one failure and two passing. Check the failure first to save needless checking. Because the password is only 8 characters I have no qualms about separating the checks into their own loops.

### Now vs Then

Wow! Old me found something current me forgot! I can use `strpbrk` to search a string for bytes from a string. I had remembered `strtok` for checking a string for a single character but `strpbrk` does it for strings. Way to go!

Other than utilizing niche standard library functions, the techniques are very similar. I iterate right in the `do-while` loop and combine the checks for clause 1 and 3 in a single loop. I prefer my new code as its easier to reason about the separate parts.

### Library Notes

No library usage today!
