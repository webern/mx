use snafu::{Backtrace, Snafu};

pub(crate) type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Snafu)]
#[snafu(visibility = "pub(crate)")]
pub(crate) enum Error {
    #[snafu(display("Command failed to run {}: {}", message, backtrace))]
    CommandExec {
        message: String,
        backtrace: Backtrace,
    },
}
