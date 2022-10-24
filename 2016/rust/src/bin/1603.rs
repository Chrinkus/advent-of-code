use std::io;

fn main() {
    let input = read_input();

    let part1 = count_triangles_1(&input);
    println!("Part 1: {part1}");
    let part2 = count_triangles_2(&input);
    println!("Part 2: {part2}");
}

fn read_input() -> Vec<[i32;3]> {
    let mut v = Vec::new();
    let lines = io::stdin().lines();

    for line in lines {
        let line = line.unwrap();
        let mut it = line
            .split_whitespace()
            .map(|s| s.parse::<i32>().unwrap());

        v.push([it.next().unwrap(),
                it.next().unwrap(),
                it.next().unwrap()]);
    }
    v
}

fn count_triangles_1(input: &Vec<[i32;3]>) -> i32 {
    let mut count = 0;
    for arr in input.iter() {
        if is_triangle(*arr) {
            count += 1;
        }
    }
    count
}

fn is_triangle(a: [i32;3]) -> bool {
    a[0] + a[1] > a[2] &&
    a[0] + a[2] > a[1] &&
    a[1] + a[2] > a[0]
}

fn count_triangles_2(input: &Vec<[i32;3]>) -> i32 {
    let mut count = 0;
    for i in (0..input.len()).step_by(3) {
        let a1 = input[i];
        let a2 = input[i+1];
        let a3 = input[i+2];

        for j in 0..3 {
            if is_triangle([a1[j], a2[j], a3[j]]) {
                count += 1;
            }
        }
    }
    count
}
