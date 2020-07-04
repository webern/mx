use crate::xsd::EntryType;
use std::cmp::Ordering;
use std::fmt::{Display, Formatter};

#[derive(Clone, Debug, Eq, PartialEq, Ord)]
pub struct Id {
    entry_type: EntryType,
    name: String,
}

impl Id {
    pub fn new(entry_type: EntryType, name: String) -> Self {
        Self { entry_type, name }
    }

    pub fn name(&self) -> Option<&str> {
        Some(self.name.as_str())
    }
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
