import * as readline from 'node:readline';
import { stdin } from 'node:process';
const rl = readline.createInterface(stdin);

const MAX_ROLLS = 3;
let grid = [];

rl.on('line', (input) => {
    grid.push([...input.trim()]);
});

// Is there a better way to get the neighbouring-8 spaces around a given set
// of coordinates? Should one function just return an array of valid x,y pairs
// and another do the checking?
function countCharsAround(x, y, grid, ch) {
    let count = 0;
    for (let i = y - 1; i <= y + 1; ++i) {
        if (i < 0 || grid.length <= i)          // range check
            continue;
        for (let j = x - 1; j <= x + 1; ++j) {
            if (j < 0 || grid[0].length <= j)   // range check
                continue;
            if (i === y && j === x)             // self check
                continue;

            if (grid[i][j] === ch)
                ++count;
        }
    }
    return count;
}

function countRemovableRolls(grid) {
    let rolls = 0;

    for (let i = 0; i < grid.length; ++i) {
        for (let j = 0; j < grid[0].length; ++j) {
            if (grid[i][j] !== '@')
                continue;
            if (countCharsAround(j, i, grid, '@') <= MAX_ROLLS) {
                ++rolls;
            }
        }
    }
    return rolls;
}

function countAndRemoveRolls(grid) {
    let rolls = 0;
    let newGrid = [];

    for (let i = 0; i < grid.length; ++i) {
        let newRow = [...grid[i]];
        for (let j = 0; j < grid[0].length; ++j) {
            if (grid[i][j] !== '@')
                continue;
            if (countCharsAround(j, i, grid, '@') <= MAX_ROLLS) {
                ++rolls;
                newRow[j] = '.';
            }
        }
        newGrid.push(newRow);
    }
    return { rolls, newGrid };
}

function removeRollsUntilDone(grid) {
    let sum = 0;

    let rolls = 0, newGrid = [];
    do {
        ({ rolls, newGrid } = countAndRemoveRolls(grid));
        sum += rolls;
        grid = newGrid;
    } while (rolls !== 0);

    return sum;
}

rl.on('close', () => {
    let part1 = countRemovableRolls(grid);
    let part2 = removeRollsUntilDone(grid);

    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
