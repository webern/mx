extern crate env_logger;

use std::path::PathBuf;
use std::str::FromStr;

use snafu::{Backtrace, GenerateBacktrace, ResultExt};
use structopt::StructOpt;

use crate::error::{Error, Result};

#[derive(Debug, StructOpt)]
pub struct GenerateArgs {
    /// Path to musicxml.xsd
    #[structopt(short = "x", long = "xsd")]
    xsd: PathBuf,
}

impl GenerateArgs {
    pub(crate) fn run(&self) -> Result<()> {
        trace!("args: {:?}", self);
        let g = Generator::new(
            self.xsd.clone(),
            Language::Cpp {
                outdir: PathBuf::from_str(".")?,
            });
        g.generate()
    }
}

pub enum Language {
    Rust,
    Cpp {
        outdir: PathBuf,
    },
}

pub struct Generator {
    xsd: PathBuf,
    language: Language,
}

impl Generator {
    fn new(xsd: PathBuf, language: Language) -> Generator {
        Self {
            xsd,
            language,
        }
    }

    fn generate(&self) -> Result<()> {
        info!("doing the generate thing");
        Err(Error::CommandExec {
            message: "generate".to_string(),
            backtrace: Backtrace::generate(),
        })
        // Ok(())
    }
}