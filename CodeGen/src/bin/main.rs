use libmxgen::generate::{GenArgs, run};

fn main() {
    std::process::exit(match run(GenArgs::default()) {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            1
        }
    })
}
