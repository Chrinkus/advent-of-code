use std::io;
use std::collections::HashMap;
use std::cmp::Reverse;

#[derive(Debug)]
struct RoomData {
    name: String,
    id: usize,
    check: [char;5],
}

impl RoomData {
    fn new(s: &String) -> Self {
        let (name, rest) = s
            .rsplit_once('-')
            .unwrap();
        let name = name.to_string().replace("-", " ");

        let i = rest.find('[').unwrap();
        let id: usize = rest[0..i].parse().expect("Unable to parse");
        let check: [char;5] = rest[i+1..i+6]
            .chars()
            .collect::<Vec<char>>()
            .try_into()
            .unwrap();

        Self { name, id, check }
    }

    fn decrypt_name(&self) -> String {
        let mut name = String::new();
        let shift: u8 = (self.id % 26).try_into().unwrap();
        for c in self.name.chars() {
            name.push(shift_char(c, shift));
        }
        name
    }
}

fn main() {
    let rooms = process_input();

    let part1 = rooms.iter().fold(0, |acc, x| acc + x.id);
    println!("Part 1: {part1}");

    let part2 = find_north_pole_objects(&rooms);
    println!("Part 2: {part2}");
}

fn process_input() -> Vec<RoomData> {
    io::stdin()
        .lines()
        .map(|line| RoomData::new(&line.unwrap()))
        .filter(|r| is_valid_room(r))
        .collect()
}

fn is_valid_room(r: &RoomData) -> bool {
    let charcounts = count_chars(&r.name);
    let tallies = sort_counts(&charcounts);

    'tally: for t in tallies[..5].into_iter() {
        for c in r.check.into_iter() {
            if c == *t.0 {
                continue 'tally;
            }
        }
        return false;
    }
    true
}

fn count_chars(s: &String) -> HashMap<char,i32> {
    let mut counts = HashMap::new();
    for ch in s.chars() {
        if ch.is_alphabetic() {
            let count = counts
                .entry(ch)
                .or_insert(0);
            *count += 1;
        }
    }
    counts
}

fn sort_counts(counts: &HashMap<char,i32>) -> Vec<(&char,&i32)> {
    let mut v = counts.iter().collect::<Vec<(&char,&i32)>>();
    v.sort_unstable_by_key(|tup| (Reverse(tup.1), tup.0));
    v
}

fn find_north_pole_objects(rooms: &Vec<RoomData>) -> usize {
    for r in rooms.iter() {
        let s = r.decrypt_name();
        if s.contains("north") {
            return r.id;
        }
    }
    0
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Could be re-used elsewhere
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
const ASCII_LOWER_A: u8 = 'a' as u8;
const ASCII_LOWER_Z: u8 = 'z' as u8;
const ASCII_UPPER_A: u8 = 'A' as u8;
const ALPHA_SIZE: u8 = ASCII_LOWER_Z - ASCII_LOWER_A + 1;

fn shift_char(ch: char, n: u8) -> char {
    if !ch.is_ascii_alphabetic() {
        return ch;
    }
    let base = if ch.is_ascii_lowercase() {
        ASCII_LOWER_A
    } else {
        ASCII_UPPER_A
    };
    let cur_offset = ch as u8 - base;
    let new_offset = (cur_offset + n) % ALPHA_SIZE;
    ((new_offset + base) as u8) as char
}
