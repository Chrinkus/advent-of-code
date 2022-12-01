# Advent of Code 2022

Another year, another wonderful event! This year I will be solving problems in C and Rust at the start. A significant change over previous years is that I will not be attempting the solutions as they release. Rather I will be valuing my sleep and attempting the problems the following day on my lunch break.

Something else I'd like to do is keep a running journal of my thought process each day. Returning to problems in previous years I have found that my current thinking is different at some points and I would like to have a more literal record of my process than just the code.

## Journal

### Day 1 - Calorie Counting

The challenge here was the read. The input consists of groups of single integer lines separated by a blank line. We are to add up the integers in each group and record the sum. Which ever sum is the greatest is the answer to part 1. Part 2 just takes the sum of the top 3.

I needed to quickly add a reverse-integer-sort to my library for sorting from largest to smallest. Some further possible additions to the library are a max-element function and possibly a function to get the sum of a vector slice.
