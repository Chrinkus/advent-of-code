# The C Journal

Here we go again. Let's see how far I get.

## Day 1 - Historian Hysteria

It's been a year so day 1 was all about remembering how to write C and use my library. I vaguely remember looking into auto documenting the code with Doxygen. I should really get that done.

Sort! This challenge is a fine example of Eric Wastl directing you to perform a simple, everyday function without explicitly telling you which one.

## Day 2 - Red-Nosed Reports

OOF! This one hit a little harder than I expected. My programming brain was focused on including checks for odd data such as lines with single values or rampant repetition. Especially in part 2 when we remove an element from the line.

Then there's the C paradigm of checking return results every allocation. That adds lines and conditionals. Is it important to keep doing this? Only when something fails, then I'll be able to tell exactly where.

Both my string and vector usage highlight one of the main drawbacks of my C use. These allocations are not cheaply discarded so running loops without leaking requires my `_clear()` functions to reuse containers rather than creating new ones every iteration. Is this okay?
