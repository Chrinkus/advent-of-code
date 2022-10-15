use std::io;

fn main() {
    let input = read_input();

    let part1 = run_jumps(&input, adjust_offset_1);
    println!("Part 1: {part1}");

    let part2 = run_jumps(&input, adjust_offset_2);
    println!("Part 2: {part2}");
}

fn read_input() -> Vec<i32> {
    let mut v: Vec<i32> = Vec::new();

    let lines = io::stdin().lines();
    for line in lines {
        let n: i32 = line
            .unwrap()
            .trim()
            .parse()
            .expect("Failed to parse");
        v.push(n);
    }
    v
}

fn run_jumps(input: &Vec<i32>, adj: fn(i32) -> i32) -> i32 {
    let mut count = 0;
    let mut v = input.clone();
    let mut i: i32 = 0;

    while 0 <= i && i < input.len().try_into().unwrap() {
        let index: usize = i.try_into().unwrap();
        let n = &mut v[index];
        i += *n;
        *n += adj(*n);
        count += 1;
    }

    count
}

fn adjust_offset_1(_n: i32) -> i32 {
    1
}

fn adjust_offset_2(n: i32) -> i32 {
    if n >= 3 { -1 } else { 1 }
}
