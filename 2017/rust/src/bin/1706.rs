use std::io;
use std::collections::BTreeMap;

fn main() {
    let input = read_initial_state();

    let (part1, part2) = run_reallocations(&input);

    println!("Part 1: {part1}");
    println!("Part 2: {part2}");
}

fn read_initial_state() -> Vec<i32> {
    let mut buff = String::new();
    io::stdin()
        .read_line(&mut buff)
        .expect("Failed to read input");

    let v: Vec<i32> = buff
        .trim()
        .split_whitespace()
        .map(|s| s.parse().expect("Parse error"))
        .collect();

    v
}

fn run_reallocations(input: &Vec<i32>) -> (i32, i32) {
    let mut cycles = 0;
    let mut seen = BTreeMap::new();
    let mut mem = input.clone();

    loop {
        let s: String = mem
            .iter()
            .map(ToString::to_string)
            .collect();
        if seen.contains_key(&s) {
            let val = seen.get(&s).unwrap();
            return (cycles, cycles - val);
        }

        seen.insert(s, cycles);
        reallocate(&mut mem);
        cycles += 1;
    }
}

fn reallocate(mem: &mut Vec<i32>) {
    let (mut i, max) = mem
        .iter()
        .enumerate()
        .rev()              // need first instance
        .max_by_key(|(_, ele)| *ele).unwrap();
    let mut n = *max;
    mem[i] = 0;

    while n > 0 {
        i = (i + 1) % mem.len();
        mem[i] += 1;
        n -= 1;
    }
}
