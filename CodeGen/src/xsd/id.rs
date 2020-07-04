use crate::xsd::EntryType;
use std::cmp::Ordering;
use std::fmt::{Display, Formatter};

#[derive(Clone, Debug, Eq, PartialEq, Ord)]
pub struct Id {
    pub entry_type: EntryType,
    pub name: String,
}

impl Display for Id {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}:{}", self.entry_type, self.name)
    }
}

impl PartialOrd for Id {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if self.entry_type != other.entry_type {
            return Some(self.entry_type.cmp(&other.entry_type));
        }
        Some(self.name.cmp(&other.name))
    }
}
