use std::io;

fn main() {
    let (part1, part2) = process_input();

    println!("Part 1: {part1}");
    println!("Part 2: {part2}");
}

fn process_input() -> (i32, i32) {
    let mut part1 = 0;
    let mut part2 = 0;

    let lines = io::stdin().lines();
    for line in lines {
        let s: String = line.unwrap();      // line is temporary(?)
        let mut words: Vec<&str> = s
            .trim()
            .split_whitespace()
            .collect();

        words.sort();

        part1 += validate_dups(&words);
        part2 += validate_anas(&words);
    }

    (part1, part2)
}

fn validate_dups(v: &Vec<&str>) -> i32 {
    for (i, w) in v.iter().enumerate() {
        if i > 0 && w == &v[i-1] {          // ugly
            return 0;
        }
    }
    1
}

fn validate_anas(v: &Vec<&str>) -> i32 {
    let mut vs: Vec<String> = Vec::new();

    for word in v.iter() {
        // sort the characters in the string
        let mut vc: Vec<char> = word
            .chars()
            .collect();
        vc.sort();

        let s = String::from_iter(vc);
        vs.push(s);
    }
    vs.sort();

    // to match signature of validate_dups
    let vstr: Vec<&str> = vs.iter().map(AsRef::as_ref).collect();
    validate_dups(&vstr)
}

