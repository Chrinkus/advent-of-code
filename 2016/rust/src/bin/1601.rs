use std::io;

#[derive(Debug,Clone,Copy)]
enum Turn {
    Left,
    Right,
}

#[derive(Debug)]
struct Instruction {
    t: Turn,
    n: i32,
}

impl Instruction {
    fn new(s: &str) -> Self {
        let mut it = s.chars();
        let t = match it.next().unwrap() {
            'L' => Turn::Left,
            'R' => Turn::Right,
            _   => panic!("Unknown turn!"),
        };
        let n = it.collect::<String>().parse::<i32>().unwrap();
        Self { t, n }
    }
}

#[derive(Debug,Clone,Copy)]
enum Facing {
    North,
    East,
    South,
    West,
}

impl Facing {
    fn rotate_cw(&self) -> Facing {
        match self {
            Facing::North   => Facing::East,
            Facing::East    => Facing::South,
            Facing::South   => Facing::West,
            Facing::West    => Facing::North,
        }
    }

    fn rotate_ccw(&self) -> Facing {
        match self {
            Facing::North   => Facing::West,
            Facing::East    => Facing::North,
            Facing::South   => Facing::East,
            Facing::West    => Facing::South,
        }
    }
}

#[derive(Debug)]
struct Position {
    x: i32,
    y: i32,
    f: Facing,
}

impl Position {
    fn new() -> Self {
        Self {
            x: 0,
            y: 0,
            f: Facing::North,
        }
    }

    fn turn(&mut self, t: Turn) {
        self.f = match t {
            Turn::Left  => self.f.rotate_ccw(),
            Turn::Right => self.f.rotate_cw(),
        };
    }

    fn advance(&mut self, n: i32) {
        match self.f {
            Facing::North   => self.y -= n,
            Facing::East    => self.x += n,
            Facing::South   => self.y += n,
            Facing::West    => self.x -= n,
        }
    }

    fn distance_from_origin(&self) -> i32 {
        self.x.abs() + self.y.abs()
    }
}

fn main() {
    let input = read_instructions();
    let part1 = follow_instructions(&input);
    println!("Part 1: {part1}");
}

fn read_instructions() -> Vec<Instruction> {
    let mut buff = String::new();
    io::stdin()
        .read_line(&mut buff)
        .expect("Failed to read input");

    buff.trim()
        .split(", ")
        .map(|s| Instruction::new(s))
        .collect()
}

fn follow_instructions(vi: &Vec<Instruction>) -> i32 {
    let mut pos = Position::new();
    for ins in vi.iter() {
        pos.turn(ins.t);
        pos.advance(ins.n);
    }
    pos.distance_from_origin()
}
