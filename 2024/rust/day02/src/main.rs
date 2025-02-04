use std::io;

fn main() {
    let reports = parse_input();

    let (part_1, part_2) = count_safe_reports(&reports);
    println!("Part 1: {part_1}");
    println!("Part 2: {part_2}");
}

fn parse_input() -> Vec<Vec<i32>> {
    let mut reports = Vec::new();
    let input = std::io::read_to_string(io::stdin())
        .expect("Could not read input");

    let lines: Vec<&str> = input.trim().split('\n').collect();
    
    for line in lines {
        let mut nums: Vec<i32> = Vec::new();

        let strnum: Vec<&str> = line.trim().split_whitespace().collect();
        for num in strnum {
            nums.push(num.parse().expect("Could not parse number"));
        }
        reports.push(nums);
    }

    reports
}

fn count_safe_reports(reports: &Vec<Vec<i32>>) -> (i32, i32) {
    let mut sum_1 = 0;
    let mut sum_2 = 0;

    for report in reports {
        if is_safe(report) {
            sum_1 += 1;
            sum_2 += 1;
        } else if is_dampened_safe(report) {
            sum_2 += 1;
        }
    }

    (sum_1, sum_2)
}

fn is_safe(report: &Vec<i32>) -> bool {
    let a = report[0];
    let b = report[1];
    if a == b { return false; }

    goes_up_safe(report) || goes_down_safe(report)
}

fn goes_up_safe(report: &Vec<i32>) -> bool {
    for i in 1..report.len() {
        let a = report[i-1];
        let b = report[i];
        if b <= a || b - a > 3 { return false; }
    }
    true
}

fn goes_down_safe(report: &Vec<i32>) -> bool {
    for i in 1..report.len() {
        let a = report[i-1];
        let b = report[i];
        if b >= a || a - b > 3 { return false; }
    }
    true
}

fn is_dampened_safe(report: &Vec<i32>) -> bool {
    for i in 0..report.len() {
        let mut r = report.clone();
        r.remove(i);
        if is_safe(&r) { return true; }
    }
    false
}
