use crate::error::Result;
use crate::xsd::constants::{
    ANNOTATION, ATTRIBUTE_GROUP, COMPLEX_TYPE, ELEMENT, GROUP, IMPORT, SIMPLE_TYPE,
};
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

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub enum EntryType {
    Annotation,
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    Import,
    SimpleType,
    Other(String),
}

impl Display for EntryType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            EntryType::Annotation => ANNOTATION,
            EntryType::AttributeGroup => ATTRIBUTE_GROUP,
            EntryType::ComplexType => COMPLEX_TYPE,
            EntryType::Element => ELEMENT,
            EntryType::Group => GROUP,
            EntryType::Import => IMPORT,
            EntryType::SimpleType => SIMPLE_TYPE,
            EntryType::Other(s) => s.as_str(),
        };
        write!(f, "{}", s)
    }
}

impl EntryType {
    pub fn parse<S: AsRef<str>>(s: S) -> Result<EntryType> {
        let et = match s.as_ref() {
            ANNOTATION => EntryType::Annotation,
            ATTRIBUTE_GROUP => EntryType::AttributeGroup,
            COMPLEX_TYPE => EntryType::ComplexType,
            ELEMENT => EntryType::Element,
            GROUP => EntryType::Group,
            IMPORT => EntryType::Import,
            SIMPLE_TYPE => EntryType::SimpleType,
            _ => EntryType::Other(s.as_ref().into()),
        };
        Ok(et)
    }
}
