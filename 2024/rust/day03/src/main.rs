use std::io;
use regex::Regex;

fn main() {
    let input = io::read_to_string(io::stdin())
        .expect("Could not read input");

    let re = Regex::new(r"mul\(([0-9]+),([0-9]+)\)").unwrap();

    let mut part_1: u32 = 0;
    for (_, [x, y]) in re.captures_iter(&input).map(|c| c.extract()) {
        let x: u32 = x.parse().unwrap();
        let y: u32 = y.parse().unwrap();
        part_1 += x * y;
    }

    println!("Part 1: {part_1}");
}
