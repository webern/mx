use structopt::StructOpt;

use libmxgen::error::Result;
use libmxgen::generate;

fn main() {
    std::process::exit(match generate::run(generate::GenArgs::default()) {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            1
        }
    })
}
