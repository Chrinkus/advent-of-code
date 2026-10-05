import * as readline from 'node:readline';
import { stdin } from 'node:process';

const rl = readline.createInterface(stdin);

const RED = 12;
const GREEN = 13;
const BLUE = 14;

const game = (s) => {
    const [ g, p ] = s.split(':');
    const id = Number(g.split(' ')[1]);

    let pulls = [];
    p.split(';').forEach((pull) => {
        let data = { 'red': 0, 'green': 0, 'blue': 0 };
        pull.split(',').forEach((type) => {
            const [ q, c ] = type.trim().split(' ');
            data[c] = Number(q);
        });
        pulls.push(data);
    });

    return {
        getId: () => id,
        isValid: () => {
            for (let i = 0; i < pulls.length; ++i) {
                if (pulls[i]['red'] > RED ||
                    pulls[i]['green'] > GREEN ||
                    pulls[i]['blue'] > BLUE)
                    return false;
            }
            return true;
        },
        getPower: () => {
            let r = 0, g = 0, b = 0;
            pulls.forEach((p) => {
                r = Math.max(r, p['red']);
                g = Math.max(g, p['green']);
                b = Math.max(b, p['blue']);
            });
            return r * g * b;
        }
    };
};

let part1 = 0;
let part2 = 0;

rl.on('line', (line) => {
    const g = game(line);

    if (g.isValid())
        part1 += g.getId();

    part2 += g.getPower();
});

rl.on('close', () => {
    console.log(`Part 1: ${part1}`);
    console.log(`Part 2: ${part2}`);
});
