import * as readline from 'node:readline';
import { stdin } from 'node:process';

const rl = readline.createInterface(stdin);

let part1 = 0;
let part2 = 0;

const NUMS = [
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine",
];

function strcmp(s1, s2) {
    for (let i = 0; i < s1.length; ++i) {
        if (i >= s2.length)
            return 1;   // strings have been equal but s1 is longer, thus later
        if (s1[i] !== s2[i])
            return s1[i] - s2[i];
    }
    return 0;
}

function getNumString(s) {
    for (let i = 0; i < NUMS.length; ++i)
        if (strcmp(NUMS[i], s) === 0)
            return i;
    return -1;
}

function isDigit(c) {
    return '0' <= c && c <= '9';
}

function detectCalibrationValue(s) {
    let i = 0, j = s.length - 1;
    for (; i < s.length; ++i)
        if (isDigit(s[i]))
            break;
    for (; j >= 0; --j)
        if (isDigit(s[j]))
            break;
    return Number(s[i] + s[j]);
}

function detectCalibrationValue2(s) {
    let tens = -1, ones = -1;

    for (let i = 0; i < s.length; ++i) {
        if (isDigit(s[i])) {
            tens = Number(s[i]);
            break;
        }
        tens = getNumString(s.substring(i));
        if (tens !== -1)
            break;
    }

    for (let i = s.length - 1; i >= 0; --i) {
        if (isDigit(s[i])) {
            ones = Number(s[i]);
            break;
        }
        ones = getNumString(s.substring(i));
        if (ones !== -1)
            break;
    }

    return tens * 10 + ones;
}


rl.on('line', (line) => {
    part1 += detectCalibrationValue(line);
    part2 += detectCalibrationValue2(line);
});

rl.on('close', () => {
    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
