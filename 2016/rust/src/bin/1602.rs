use std::io;

static KEYPAD_1: [[char;3];3] = [
    ['1', '2', '3'],
    ['4', '5', '6'],
    ['7', '8', '9'],
];

static DUM: char = ' ';
static KEYPAD_2: [[char;5];5] = [
    [DUM, DUM, '1', DUM, DUM],
    [DUM, '2', '3', '4', DUM],
    ['5', '6', '7', '8', '9'],
    [DUM, 'A', 'B', 'C', DUM],
    [DUM, DUM, 'D', DUM, DUM],
];

struct Position {
    x: usize,
    y: usize,
}

fn main() {
    let input: Vec<String> = io::stdin()
        .lines()
        .map(|line| line.unwrap().clone())
        .collect();

    let part1 = follow_instructions_1(&input);
    println!("Part 1: {part1}");
    let part2 = follow_instructions_2(&input);
    println!("Part 2: {part2}");
}

fn follow_instructions_1(vs: &Vec<String>) -> String {
    let mut code = String::new();
    let mut pos = Position { x: 1, y: 1 };
    for s in vs.iter() {
        for c in s.chars() {
            match c {
                'U' => if pos.y > 0 { pos.y -= 1; },
                'D' => if pos.y < 2 { pos.y += 1; },
                'L' => if pos.x > 0 { pos.x -= 1; },
                'R' => if pos.x < 2 { pos.x += 1; },
                _   => panic!("Unknown char: '{}'", c),
            }
        }
        code.push(KEYPAD_1[pos.y][pos.x]);
    }
    code
}

fn follow_instructions_2(vs: &Vec<String>) -> String {
    let mut code = String::new();
    let mut pos = Position { x: 0, y: 2 };
    for s in vs.iter() {
        for c in s.chars() {
            match c {
                'U' => {
                    if pos.y > 0 && KEYPAD_2[pos.y-1][pos.x] != DUM {
                        pos.y -= 1;
                    }
                },
                'D' => {
                    if pos.y < 4 && KEYPAD_2[pos.y+1][pos.x] != DUM {
                        pos.y += 1;
                    }
                }
                'L' => {
                    if pos.x > 0 && KEYPAD_2[pos.y][pos.x-1] != DUM {
                        pos.x -= 1;
                    }
                }
                'R' => {
                    if pos.x < 4 && KEYPAD_2[pos.y][pos.x+1] != DUM {
                        pos.x += 1;
                    }
                }
                _   => panic!("Unknown char: '{}'", c),
            }
        }
        code.push(KEYPAD_2[pos.y][pos.x]);
    }

    code
}
