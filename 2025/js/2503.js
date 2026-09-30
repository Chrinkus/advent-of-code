const readline = require('node:readline');
const rl = readline.createInterface(process.stdin);

let part1 = 0;
let part2 = 0;

function getMaxJoltage(b, n) {
    let jolts = 0;
    let index = 0;

    while (n--) {           // need to immediately decrement for inner loop
        let max = 0;

        for (let i = index; i < b.length - n; ++i) {
            if (Number(b[i]) > max) {
                max = Number(b[i]);
                index = i;
            }
        }

        jolts = jolts * 10 + max;   // better than reconstructing a string
        ++index;                    // next iter starts on element after max
    }
    return jolts;
}

rl.on('line', (input) => {
    part1 += getMaxJoltage(input, 2);
    part2 += getMaxJoltage(input, 12);
});

rl.on('close', () => {
    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
