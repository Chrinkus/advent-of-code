const readline = require('node:readline');
const rl = readline.createInterface(process.stdin);

const DIAL = 100;   // number of possible dial values, 0-99
let place = 50;     // given as starting point
let part1 = 0;      // count of times dial lands on zero
let part2 = 0;      // count of times dial passes or lands on zero

rl.on('line', (input) => {
    const m = Number(input.slice(1));
    const n = m % DIAL;
    const prev = place;             // need this to check zero starts later

    part2 += Math.floor(m / DIAL);  // catch all excessive turns

    switch (input[0]) {
        case 'L':   place -= n; break;
        case 'R':   place += n; break;
    }

    // this was the last piece. positive overflow was checked correctly but
    // negative underflow was NOT a zero-pass when starting from zero!
    if ((place <= 0 && prev != 0) || DIAL <= place)
        ++part2;

    place = (place + DIAL) % DIAL;  // correct dial overflow/underflow
    if (place === 0)
        ++part1;
});

rl.on('close', () => {
    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
