use std::io;

fn main() {
    let v: Vec<i32> = read_digits();

    let part1 = next_match_sum(&v);
    println!("Part 1: {part1}");

    let part2 = half_wrap_sum(&v);
    println!("Part 2: {part2}");
}

fn read_digits() -> Vec<i32> {
    let mut v: Vec<i32> = Vec::new();
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read input");

    for c in input.trim().chars() {
        v.push(c.to_digit(10).unwrap().try_into().unwrap());
    }

    v
}

fn next_match_sum(v: &Vec<i32>) -> i32 {
    let mut sum = 0;
    let mut prev = v[0];

    for n in &v[1..] {
        if *n == prev {
            sum += prev;
        }
        prev = *n;
    }

    if v[0] == prev {
        sum += prev;
    }

    sum
}

fn half_wrap_sum(v: &Vec<i32>) -> i32 {
    let mut sum = 0;
    let half = v.len() / 2;

    for (i, n) in v[..half].iter().enumerate() {
        if *n == v[i+half] {
            sum += *n * 2;
        }
    }

    sum
}
