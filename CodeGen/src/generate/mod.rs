extern crate env_logger;

use std::path::PathBuf;

use snafu::{Backtrace, GenerateBacktrace};
use structopt::StructOpt;

use crate::error::{Error, Result};

#[derive(Debug, StructOpt)]
pub(crate) struct GenerateArgs {
    /// Path to musicxml.xsd
    #[structopt(short = "x", long = "xsd")]
    xsd: PathBuf,
}

impl GenerateArgs {
    pub(crate) fn run(&self) -> Result<()> {
        trace!("args: {:?}", self);
        Err(Error::CommandExec {
            message: "generate".to_string(),
            backtrace: Backtrace::generate(),
        })
    }
}