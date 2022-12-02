use std::io;

fn main() -> io::Result<()> {
    let mut cals = Vec::new();
    let mut buff = String::new();

    let mut calories = 0;
    while io::stdin().read_line(&mut buff)? != 0 {
        match buff.trim().parse::<i32>() {
            Ok(n) => calories += n,
            Err(_) => {
                cals.push(calories);
                calories = 0;
            }
        }
        buff.clear();
    }
    cals.push(calories);

    let part1: i32 = cals.iter().max().unwrap();    // Leave it for learning
    println!("{part1}");

    cals.sort_unstable_by(|a, b| b.cmp(a));     // Sort negates need for max
    let part2: i32 = cals[..3].iter().sum();
    println!("{part2}");

    Ok(())
}
