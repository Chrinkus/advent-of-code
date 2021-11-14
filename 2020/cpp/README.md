# Advent of Code 2020

## Goals for this year:
- Create some visualizations
- Try to find alternatives to `std::regex`ing all the things
- Try to stream data to the solutions as much as possible
- Run hard at graph traversal, not away from..
- Most importantly have fun and don't lose too much sleep trying to get solutions done before going to bed!

## Description of Puzzles
I often want to go back and look at a puzzle several months after the event but have a tough time locating the specific puzzle. Hopefully this list is helpful to future me.

### Day 1 - Report Repair
---
#### Input
- Random integers in the range of 1 to 2020, unsorted

#### Part 1
- Find the two entries that sum to 2020
- Multiply the two entries

#### Part 2
- Find three entries that sum to 2020
- Multiply the three entries

### Day 2 - Password Philosophy
---
#### Input
- Passwords in the form `1-3 a: abcde`

#### Part 1
- Count the valid passwords according to a policy
- The range `1-3` indicates the lowest and highest number of times `a` must appear in the password

#### Part 2
- Count the valid passwords according to a different policy
- The digits `1` and `3` are the locations in the password that the letter `a` must appear once
    - Either at the first location or second but not both
- Not zero-indexed, first character is position 1

### Day 3 - Toboggan Trajectory
---
#### Input
- 2D grid of trees '#' and open space '.'
- Long in the y, short but repeating in the x

#### Part 1
- Given a slope for down-right traversal, how many trees would be hit before getting to the bottom

#### Part 2
- Calculate the number of trees encountered when traversing multiple slopes
    - multiply the results together

### Day 4 - Passport Processing
---
#### Input
- Data from multiple passports split irregularly across lines
    - Each individual passport is separated by an empty line

#### Part 1
- Count the number of passports with the required fields
    - Required fields provided in problem text

#### Part 2
- Count the number of passports with the required fields AND have valid data
    - Constraints provided in problem text

### Day 5 - Binary Boarding
---
#### Input
- Lines of characters providing binary directions to seats
    - Such as `BBFFBBFRLL` where the first 7 indicate row and last 3 indicate column

#### Part 1
- Calculate the highest seat ID
    - Row * row-width + column

#### Part 2
- Find the only empty seat