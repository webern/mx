use libmxgen::generate::{run, GenArgs};
use std::time::Instant;

fn main() {
    let start = Instant::now();
    let exit_code = match run(GenArgs::default()) {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            1
        }
    };
    let duration = start.elapsed();
    println!("Runtime: {:?}", duration);
    std::process::exit(exit_code);
}
