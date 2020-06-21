////////////////////////////////////////////////////////////////////////////////////////////////////
// public error type
////////////////////////////////////////////////////////////////////////////////////////////////////

use core::fmt;
use std::convert::Infallible;
use std::fmt::{Display, Formatter};

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    Message(MessageError),
}

impl Display for crate::error::Error {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            Error::Message(me) => me.fmt(f),
        }
    }
}

impl std::error::Error for crate::error::Error {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Error::Message(e) => {
                if let Some(s) = &e.source {
                    Some(s.as_ref())
                } else {
                    None
                }
            }
        }
    }
}

#[derive(Debug)]
pub struct MessageError {
    pub throw_site: ThrowSite,
    pub message: Option<String>,
    pub source: Option<Box<dyn std::error::Error>>,
}

/// The Rust sourcecode file and line number which is the 'throw' site of an error.
#[derive(Debug, Clone, Eq, PartialOrd, PartialEq, Hash, Default)]
pub struct ThrowSite {
    /// The rust source file where the error was thrown, i.e. file!()
    pub file: String,
    /// The rust source line number where the error was thrown, i.e. line!()
    pub line: u32,
}

impl Display for ThrowSite {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}:{}", self.file, self.line)
    }
}

impl Display for MessageError {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        self.throw_site.fmt(f)?;
        if let Some(msg) = &self.message {
            if !msg.is_empty() {
                write!(f, " - {}", msg)?;
            } else {
                write!(f, " - error")?;
            }
        }
        if let Some(e) = &self.source {
            write!(f, " - caused by: {}", e)?;
        }
        Ok(())
    }
}

/// Not sure what this is for.
impl From<Infallible> for Error {
    fn from(_: Infallible) -> Self {
        unreachable!()
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// internal macros
////////////////////////////////////////////////////////////////////////////////////////////////////

pub fn box_err<E>(err: Option<E>) -> Option<Box<dyn std::error::Error>>
    where
        E: std::error::Error + 'static,
{
    match err {
        None => None,
        Some(e) => Some(e.into()),
    }
}

/// This macro is used internally to obtain the current file and line (in the sourcecode).
macro_rules! throw_site {
    () => {
        crate::error::ThrowSite {
            file: file!().to_owned(),
            line: line!(),
        }
    };
}

/// This macro is used internally to wrap a foreign Result type into a `crate::error::Result`.
/// The first argument is always a `Result`, and the second+ arguments are for format!()
macro_rules! wrap {
    ($e:expr) => {
        match $e {
            Ok(value) => Ok(value),
            Err(er) => {
                Err(crate::error::Error::Message(crate::error::MessageError{
                    throw_site: throw_site!(),
                    message: Some("error".into()),
                    source: crate::error::box_err(Some(er)),
                }))
            }
        }
    };
    ($e:expr, $msg:expr) => {
        match $e {
            Ok(value) => Ok(value),
            Err(er) => {
                Err(crate::error::Error::Message(crate::error::MessageError{
                    throw_site: throw_site!(),
                    message: Some($msg.into()),
                    source: crate::error::box_err(Some(er)),
                }))
            }
        }
    };
    ($e:expr, $fmt:expr, $($arg:expr),+) => {
        match $e {
            Ok(value) => Ok(value),
            Err(er) => {
                Err(crate::error::Error::Message(crate::error::MessageError{
                    throw_site: throw_site!(),
                    message: Some(format!($fmt, $($arg),+)),
                    source: crate::error::box_err(Some(er)),
                }))
            }
        }
    };
}

/// This macro is used internally to create an `Err(crate::error::Error::Message)`.
macro_rules! make_err {
    () => {
        crate::error::Error::Message(crate::error::MessageError{
            throw_site: throw_site!(),
            message: Option::<String>::None,
            source: Option::<crate::error::Error>::None,
        })
    };
    ($msg:expr) => {
        crate::error::Error::Message(crate::error::MessageError{
            throw_site: throw_site!(),
            message: Some($msg.into()),
            source: Option::<Box<dyn std::error::Error>>::None,
        })
    };
    ($fmt:expr, $($arg:expr),+) => {
        crate::error::Error::Message(crate::error::MessageError{
            throw_site: throw_site!(),
            message: Some(format!($fmt, $($arg),+)),
            source: Option::<Box<dyn std::error::Error>>::None,
        })
    };
}

macro_rules! raise {
    ($($x:tt)*) => {
        {
            Err(make_err!($($x)*))
        }
    }
}
