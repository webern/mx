use structopt::StructOpt;

use crate::error::Result;

#[macro_use]
mod error;
mod generate;
mod xsd;

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
    std::process::exit(match Command::from_args().run() {
        Ok(()) => 0,
        Err(err) => {
            eprintln!("{}", err);
            1
        }
    })
}
