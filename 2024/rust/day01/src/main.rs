use std::io;

fn main() {
    let (mut list_a, mut list_b) = parse_input();
    list_a.sort();
    list_b.sort();

    let part_1 = get_list_dist(&list_a, &list_b);
    println!("Part 1: {part_1}");

    let part_2 = get_sim_score(&list_a, &list_b);
    println!("Part 2: {part_2}");
}

fn parse_input() -> (Vec<u32>, Vec<u32>) {
    let input = std::io::read_to_string(io::stdin())
        .expect("Should have been able to read input");

    let lines: Vec<&str> = input.trim().split('\n').collect();
    let mut list_a: Vec<u32> = Vec::new();
    let mut list_b: Vec<u32> = Vec::new();

    for line in lines {
        let ids: Vec<&str> = line.split_whitespace().collect();
        list_a.push(ids[0].parse().expect("Couldn't parse id"));
        list_b.push(ids[1].parse().expect("Couldn't parse id"));
    }

    (list_a, list_b)
}

fn get_list_dist(va: &Vec<u32>, vb: &Vec<u32>) -> u32 {
    let mut sum = 0;

    for i in 0..va.len() {
        let a = va[i];
        let b = vb[i];
        sum += if a > b { a - b } else { b - a };
    }

    sum
}

fn get_sim_score(va: &Vec<u32>, vb: &Vec<u32>) -> u32 {
    let mut score = 0;

    for a in va {
        let mut count = 0;
        for b in vb {
            if a == b { count += 1; } else if b > a { break; }
        }
        score += a * count;
    }

    score
}
