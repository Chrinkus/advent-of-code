import * as readline from 'node:readline';
import { stdin } from 'node:process';

const rl = readline.createInterface(stdin);

let input = [];
rl.on('line', (line) => {
    input.push(line);
});

function doCephMaff(input) {
    let total = 0;

    let lines = input.map(line => line
        .trim()
        .split(' ')
        .filter(ele => ele)
    );

    for (let j = 0; j < lines[0].length; ++j) {
        const op = lines[lines.length-1][j];
        let acc = op === '+' ? 0 : 1;
        for (let i = 0; i < lines.length-1; ++i) {
            if (op === '+')
                acc += Number(lines[i][j]);
            else
                acc *= Number(lines[i][j]);
        }
        total += acc;
    }

    return total;
}

function doRealCephMaff(input) {
    let total = 0;

    for (let j = input[0].length; j >= 0; --j) {    // outer dec to next group
        let nums = [];
        while (--j >= 0) {                          // real decrement
            let s = "";
            for (let i = 0; i < input.length - 1; ++i)
                s += input[i][j];
            nums.push(Number(s));
            
            if (input[input.length-1][j] !== ' ')
                break;
        }
        total += input[input.length-1][j] === '+'
                ? nums.reduce((acc, cur) => acc + cur, 0)
                : nums.reduce((acc, cur) => acc * cur, 1);
    }

    return total;
}

rl.on('close', () => {
    let part1 = doCephMaff(input);
    console.log(`Part 1: ${part1}`);

    let part2 = doRealCephMaff(input);
    console.log(`Part 2: ${part2}`);
});
