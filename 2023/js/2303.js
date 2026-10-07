import * as readline from 'node:readline';
import { stdin } from 'node:process';

const rl = readline.createInterface(stdin);

let input = [];
let gears = {};         // Ew global, I know..
const NOTHING = '.';
const GEAR = '*';

rl.on('line', (line) => {
    input.push(line.trim());
});

rl.on('close', () => {
    const part1 = partScanAndSum(input);
    const part2 = getGearRatios(gears);

    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});

function partScanAndSum(input) {
    let sum = 0;

    for (let i = 0; i < input.length; ++i) {
        for (let j = 0; j < input[0].length; ++j) {
            if (!isDigit(input[i][j]))
                continue;
            const ns = getNumString(input[i], j);
            const ap = getAdjacentPoints(j, i,
                input[0].length,    // width
                input.length,       // height
                ns.length);
            const { c, y, x } = getAdjacentSymbol(input, ap);
            if (c != NOTHING)
                sum += Number(ns);
            j += ns.length - 1;

            // Part 2 stuff, adding it here since I have everything I need
            if (c === GEAR) {
                const key = `${c}:${y}:${x}`;
                if (!gears.hasOwnProperty(key))
                    gears[key] = [];
                gears[key].push(Number(ns));
            }
        }
    }

    return sum;
}

function isDigit(c) {
    return '0' <= c && c <= '9';
}

function getNumString(s, i) {
    let ns = "";
    for (; i < s.length; ++i) {
        if (!isDigit(s[i]))
            break;
        ns += s[i];
    }
    return ns;
}

function getAdjacentPoints(x, y, w, h, l = 1) {
    let ap = [];

    for (let i = y - 1; i <= y + 1; ++i) {
        if (i < 0 || h <= i)                        // y range check
            continue;
        for (let j = x - 1; j <= x + l; ++j) {
            if (j < 0 || w <= j)                    // x range check
                continue;
            if (y === i && (x <= j && j < x + l))   // self check
                continue;
            ap.push({ x: j, y: i });
        }
    }

    return ap;
}

function getAdjacentSymbol(grid, points) {
    for (let i = 0; i < points.length; ++i) {
        const c = grid[points[i].y][points[i].x];
        if (c !== NOTHING)
            return { c, y: points[i].y, x: points[i].x };
    }
    return { c: NOTHING, y: -1, x: -1 };
}

function getGearRatios(gears) {
    let sum = 0;

    for (let key in gears)
        if (gears[key].length === 2)
            sum += gears[key].reduce((acc, cur) => acc * cur);

    return sum;
}
