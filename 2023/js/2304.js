import * as readline from "node:readline";
import { stdin } from "node:process";

const rl = readline.createInterface(stdin);

const card = (data) => {
    const [ info, nums ] = data.split(':');
    const [ _, id ] = info.split(' ').filter(s => s).map(s => s.trim());
    const [ win, play ] = nums.split('|').map(s => s.trim());

    const winners = win.split(' ').filter(s => s).map(s => Number(s));
    const played = play.split(' ').filter(s => s).map(s => Number(s));
    const { points, matches } = countPoints(winners, played);

    return {
        id,             // string
        winners,        // array of numbers
        played,         // array of numbers
        matches,        // number
        points,         // number
        copies: 1,      // number
    };
}

let part1 = 0;
let cards = [];

rl.on("line", (line) => {
    const c = card(line);
    part1 += c.points;
    cards.push(c);
});

rl.on("close", () => {
    console.log(`Part 1: ${part1}`);

    countCopies(cards);
    let part2 = cards.reduce((acc, cur) => acc + cur.copies, 0);
    console.log(`Part 2: ${part2}`);
});

function countPoints(winners, played) {
    const matches = winners.filter(w => played.includes(w)).length;

    const points =  matches === 0 ? 0 : 1 << matches - 1;
    return { points, matches };
}

function countCopies(cards) {
    for (let i = 0; i < cards.length; ++i)
        for (let j = i + 1; j <= i + cards[i].matches; ++j)
            cards[j].copies += cards[i].copies;
}
