use std::io;

enum Direction {
    Forward,
    Up,
    Down,
}

struct Command {
    dir: Direction,
    val: i32,
}

impl Command {
    fn new(s: &String) -> Self {
        let mut it = s.split(' ');
        let dir = match it.next().unwrap() {
            "forward"   => Direction::Forward,
            "up"        => Direction::Up,
            "down"      => Direction::Down,
            &_          => panic!("Unknown direction!"),
        };
        let val = it.next().unwrap().parse().expect("Faild to parse");

        Self {
            dir,
            val,
        }
    }
}

struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let input = read_directions();

    let part1 = follow_course_1(&input);
    println!("Part 1: {part1}");
    let part2 = follow_course_2(&input);
    println!("Part 2: {part2}");
}

fn read_directions() -> Vec<Command> {
    let mut v = Vec::new();

    let lines = io::stdin().lines();
    for line in lines {
        let s = line.unwrap();
        let cmd = Command::new(&s);
        v.push(cmd);
    }

    v
}

fn follow_course_1(input: &Vec<Command>) -> i32 {
    let mut pt = Point { x: 0, y: 0 };
    for cmd in input.iter() {
        match cmd.dir {
            Direction::Forward  => pt.x += cmd.val,
            Direction::Up       => pt.y -= cmd.val,
            Direction::Down     => pt.y += cmd.val,
        }
    }
    pt.x * pt.y
}

fn follow_course_2(input: &Vec<Command>) -> i32 {
    let mut pt = Point { x: 0, y: 0 };
    let mut aim = 0;
    for cmd in input.iter() {
        match cmd.dir {
            Direction::Up       => aim -= cmd.val,
            Direction::Down     => aim += cmd.val,
            Direction::Forward  => {
                pt.x += cmd.val;
                pt.y += cmd.val * aim;
            },
        }
    }
    pt.x * pt.y
}
