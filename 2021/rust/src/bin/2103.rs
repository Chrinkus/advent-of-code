use std::io;

fn main() {
    let input = read_input();

    let part1 = get_power_consumption(&input);
    println!("Part 1: {part1}");
    let part2 = get_life_support_rating(&input);
    println!("Part 2: {part2}");
}

fn read_input() -> Vec<String> {
    io::stdin()
        .lines()
        .map(|line| line.unwrap())
        .collect()
}

fn get_power_consumption(report: &Vec<String>) -> i32 {
    let counts = pos_on_counts(report);
    let half = report.len() / 2;

    let mut gamma = String::new();
    let mut epsilon = String::new();
    for count in counts.iter() {
        if *count < half {
            gamma.push('0');
            epsilon.push('1');
        } else {
            gamma.push('1');
            epsilon.push('0');
        }
    }
    btoi(&gamma) * btoi(&epsilon)
}

fn pos_on_counts(report: &Vec<String>) -> Vec<usize> {
    let mut counts: Vec<usize> = vec![0; report[0].len()];
    for line in report.iter() {
        for (i, ch) in line.chars().enumerate() {
            if ch == '1' {
                counts[i] += 1;
            }
        }
    }
    counts
}

fn btoi(s: &String) -> i32 {
    i32::from_str_radix(s, 2).unwrap()
}

fn get_life_support_rating(report: &Vec<String>) -> i32 {
    filter_rating(report, get_oxy_char) * filter_rating(report, get_c02_char)
}

fn filter_rating(report: &Vec<String>, bc: fn(usize, usize) -> char) -> i32 {
    let mut lines = report.clone();
    for i in 0..lines[0].len() {
        let count = count_ones_at_pos(&lines, i);
        let ch = bc(count, lines.len());
        lines = filter_char_at_pos(lines, ch, i);

        if lines.len() == 1 {
            break;
        }
    }
    btoi(&lines[0])
}

fn get_oxy_char(count: usize, len: usize) -> char {
    if count >= len - count { '1' } else { '0' }
}

fn get_c02_char(count: usize, len: usize) -> char {
    if count < len - count { '1' } else { '0' }
}

fn count_ones_at_pos(report: &Vec<String>, pos: usize) -> usize {
    report.iter()
        .fold(0usize, |acc, ele| {
            if ele.chars().nth(pos).unwrap() == '1' {
                acc + 1
            } else {
                acc
            }
        })
}

fn filter_char_at_pos(vs: Vec<String>, ch: char, pos: usize) -> Vec<String> {
    let v = vs
        .iter()
        .filter(|s| s.chars().nth(pos).unwrap() == ch)
        .map(ToString::to_string)
        .collect();
    v
}
