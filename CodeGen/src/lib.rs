#![deny(non_camel_case_types)]
#![deny(non_snake_case)]
#![deny(non_snake_case)]
#![deny(non_upper_case_globals)]
#![allow(unused)]
#![deny(rust_2018_idioms)]
#![forbid(missing_debug_implementations, missing_copy_implementations)]

#[macro_use]
pub mod error;
pub mod generate;
pub mod xsd;
