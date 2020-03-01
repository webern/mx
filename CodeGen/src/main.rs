#[macro_use]
extern crate log;

use snafu::ErrorCompat;
use structopt::StructOpt;

use crate::error::Result;

mod error;
mod generate;

#[derive(Debug, StructOpt)]
enum Command {
    /// Generate code from musicxml.xsd
    Generate(generate::GenerateArgs),
}

impl Command {
    fn run(&self) -> Result<()> {
        match self {
            Command::Generate(args) => args.run(),
        }
    }
}

fn main() {
    env_logger::init();
    std::process::exit(match Command::from_args().run() {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            if let Some(var) = std::env::var_os("RUST_BACKTRACE") {
                if var != "0" {
                    if let Some(backtrace) = err.backtrace() {
                        eprintln!("\n{:?}", backtrace);
                    }
                }
            }
            1
        }
    })
}
