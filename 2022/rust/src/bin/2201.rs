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

    let part1 = cals.iter().max().unwrap();
    println!("{part1}");

    Ok(())
}
