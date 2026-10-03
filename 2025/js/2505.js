import * as readline from 'node:readline';
import { stdin } from 'node:process';
const rl = readline.createInterface(stdin);

let lines = [];

rl.on('line', (input) => {
    lines.push(input.trim());
});

function parseInput(input) {
    let ranges = [], ingredients = [];

    let i = 0;
    for (; ; ++i) {                                 // parse ranges
        if (input[i].length === 0)
            break;

        let [ start, end ] = input[i].split('-');
        ranges.push({ start: Number(start), end: Number(end) });
    }

    for (let j = i + 1; j < input.length; ++j) {    // parse ingredients
        ingredients.push(Number(input[j]));
    }

    return { ranges, ingredients };
}

function countFresh(ranges, ingredients) {
    let count = 0;

    for (let i = 0, j = 0; i < ingredients.length; ++i) {
        if (ingredients[i] < ranges[j].start)
            continue;       // ingredient spoiled, advance

        if (ingredients[i] <= ranges[j].end) {
            ++count;
            continue;       // ingredient fresh, count and advance
        }

        if (j < ranges.length - 1) {
            --i;            // decrement to increment back to this element
            ++j;            // check same element next range next iteration
        } else {
            break;          // past last range, all rest are spoiled
        }
    }
    return count;
}

function countFreshIDs(ranges) {
    let count = 0;

    let { start, end } = ranges[0];
    for (let i = 1; i < ranges.length; ++i) {
        if (end < ranges[i].start) {        // detect close of range
            count += end - start + 1;
            ({ start, end } = ranges[i]);
            continue;
        } else if (end < ranges[i].end) {   // open range, extend end
            end = ranges[i].end;
        }                                   // otherwise swallow smaller
    }                                       // ranges and move on

    return count + (end - start + 1);       // grab last open range
}

rl.on('close', () => {
    let { ranges, ingredients } = parseInput(lines);

    ranges.sort((a, b) => a.start - b.start);
    ingredients.sort((a, b) => a - b);

    let part1 = countFresh(ranges, ingredients);
    console.log(`Part 1: ${part1}`);

    let part2 = countFreshIDs(ranges);
    console.log(`Part 2: ${part2}`);;
});
