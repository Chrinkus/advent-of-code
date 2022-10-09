use std::io;

fn main() {
    let (part1, part2) = process_lines();
    println!("Part 1: {part1}");
    println!("Part 2: {part2}");
}

fn process_lines() -> (i32, i32) {
    let mut sum1 = 0;
    let mut sum2 = 0;

    let lines = io::stdin().lines();
    for line in lines {
        let mut v: Vec<i32> = line
            .unwrap()
            .split_whitespace()
            .map(|s| s.parse().expect("parse error"))
            .collect();

        v.sort();

        sum1 += get_diff(&v);
        sum2 += get_div(&v);
    }
    (sum1, sum2)
}

fn get_diff(v: &Vec<i32>) -> i32 {
    v[v.len()-1] - v[0]
}

fn get_div(v: &Vec<i32>) -> i32 {
    for n in v.iter().rev() {
        let half = n / 2;
        for d in v.iter() {
            if d > &half {
                break;
            } else if n % d == 0 {
                return n / d;
            }
        }
    }
    0
}
