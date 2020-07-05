pub mod builtin;
pub mod enumeration;
pub mod symbol;
use crate::model::enumeration::Enumeration;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub enum X {
    Enumeration(Enumeration),
}
