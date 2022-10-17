use std::io;

fn main() {
    let input = read_ints();

    let (part1, part2) = count_depth_incs(&input);
    println!("Part 1: {part1}");
    println!("Part 2: {part2}");
}

fn read_ints() -> Vec<i32> {
    let mut v = Vec::new();

    let lines = io::stdin().lines();
    for line in lines {
        let n = line
            .unwrap()
            .parse()
            .expect("Failed to parse");
        v.push(n);
    }

    v
}

fn count_depth_incs(input: &Vec<i32>) -> (i32, i32) {
    let mut count1 = 0;
    let mut count2 = 0;

    for (i, depth) in input.iter().enumerate() {
        if i > 0 && depth > &input[i-1] {
            count1 += 1;
        }
        if i > 2 && depth > &input[i-3] {
            count2 += 1;
        }
    }

    (count1, count2)
}

