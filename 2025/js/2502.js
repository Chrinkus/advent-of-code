const readline = require('node:readline');
const rl = readline.createInterface(process.stdin);

let part1 = 0;
let part2 = 0;

// strict - pattern is single repeat of half string length
function isInvalidStrict(id) {
    for (let i = 0, j = id.length / 2; j < id.length; ++i, ++j)
        if (id[i] != id[j])
            return false;
    return true;
}

// needed a double break or 'goto'. this solution is probably 'better'..
function checkGroups(id, pat) {
    for (let grp = 1; grp < id.length / pat; ++grp) {
        for (let i = 0, j = grp * pat; i < pat; ++i, ++j) {
            if (id[i] !== id[j])
                return false;
        }
    }
    return true;
}

// open - any length of repeating pattern
function isInvalidOpen(id) {
    for (let pat = 1; pat <= id.length / 2; ++pat) {
        if (id.length % pat !== 0)
            continue;
        if (checkGroups(id, pat))
            return true;
    }
    return false;
}

rl.on('line', (input) => {
    const ranges = input.trim().split(',');

    ranges.forEach((range) => {
        const [ start, end ] = range.split('-');

        for (let n = Number(start); n <= Number(end); ++n) {
            const s = String(n);
            if (isInvalidStrict(s))
                part1 += n;
            if (isInvalidOpen(s))
                part2 += n;
        }
    });
});

rl.on('close', () => {
    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
