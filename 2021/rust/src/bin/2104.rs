use std::io;

#[derive(Debug, Clone)]
struct Card {
    numbers: [[i32;5];5],
    bingo: bool,
}

impl Card {
    fn new() -> Self {
        let numbers = [[0;5];5];
        let bingo = false;

        Self {
            numbers,
            bingo,
        }
    }

    fn mark(&mut self, i: usize, j: usize) {
        self.numbers[i][j] = 0;
    }

    fn check(&mut self, ball: i32) -> Option<i32> {
        match self.check_ball(ball) {
            Some((i, j)) => {
                self.mark(i, j);
                if self.check_bingo(i, j) {
                    self.bingo = true;
                    Some(self.score(ball))
                } else {
                    None
                }
            },
            None => None
        }
    }

    fn check_ball(&self, ball: i32) -> Option<(usize, usize)> {
        for i in 0..self.numbers.len() {
            for j in 0..self.numbers[i].len() {
                if self.numbers[i][j] == ball {
                    return Some((i, j));
                }
            }
        }
        None
    }

    fn check_bingo(&self, i: usize, j: usize) -> bool {
        let mut row = true;
        for k in 0..self.numbers[i].len() {     // check row i
            if self.numbers[i][k] != 0 {
                row = false;
                break;
            }
        }

        let mut col = true;
        for k in 0..self.numbers.len() {        // check column j
            if self.numbers[k][j] != 0 {
                col = false;
                break;
            }
        }
        row || col
    }

    fn score(&self, ball: i32) -> i32 {
        let mut score = 0;
        for i in 0..self.numbers.len() {
            for j in 0..self.numbers[i].len() {
                score += self.numbers[i][j];
            }
        }
        score * ball
    }
}

fn main() {
    let balls = read_balls();
    let cards = read_cards();

    let part1 = get_score_of_winning_card(&balls, cards.clone());
    println!("Part 1: {part1}");

    let part2 = get_score_of_losing_card(&balls, cards.clone());
    println!("Part 2: {part2}");
}

fn read_balls() -> Vec<i32> {
    let mut buff = String::new();
    io::stdin()
        .read_line(&mut buff)
        .expect("Failed to read line");

    let balls = buff
        .trim()
        .split(',')
        .map(|s| s.parse().unwrap())
        .collect();

    balls
}

fn read_cards() -> Vec<Card> {
    let mut vc = Vec::new();

    loop {
        let card = read_card();
        match card {
            Some(c) => vc.push(c),
            None    => break,
        }
    }

    vc
}

fn read_card() -> Option<Card> {
    let mut buff = String::new();
    let mut num_bytes = io::stdin().read_line(&mut buff)    // burn a line
        .expect("Shouldn't fail..");

    if num_bytes != 1 {
        return None;
    }

    let mut card = Card::new();
    for i in 0..5 {
        buff.clear();
        num_bytes = io::stdin().read_line(&mut buff)
            .expect("Failed to read..");
        if num_bytes <= 1 {
            return None;
        }

        let vnums: Vec<i32> = buff
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("parse error"))
            .collect();

        for j in 0..5 {
            card.numbers[i][j] = vnums[j];
        }
    }

    Some(card)
}

fn get_score_of_winning_card(balls: &Vec<i32>, mut cards: Vec<Card>) -> i32 {
    for ball in balls.iter() {
        for card in cards.iter_mut() {
            match card.check(*ball) {
                Some(score) => return score,
                None => continue,
            }
        }
    }
    -1
}

fn get_score_of_losing_card(balls: &Vec<i32>, mut cards: Vec<Card>) -> i32 {
    let mut num_bingos = 0;
    let num_cards = cards.len();

    for ball in balls.iter() {
        for card in cards.iter_mut() {
            if card.bingo {
                continue;               // better solution than removing cards?
            }
            match card.check(*ball) {
                Some(score) => {
                    num_bingos += 1;
                    if num_bingos == num_cards {
                        return score;
                    }
                },
                None => continue
            }
        }
    }
    -1
}
