mod annotation;
mod simple_type;

use std::fmt::{Display, Formatter};
use crate::xsd::Entry::SimpleType;
use std::convert::TryInto;
use std::cmp::Ordering;
use std::collections::HashMap;

pub struct XSD {
    entries: HashMap<ID, Entry>,
}

pub const ANNOTATION: &str = "annotation";
pub const ATTRIBUTE_GROUP: &str = "attributeGroup";
pub const COMPLEX_TYPE: &str = "complexType";
pub const ELEMENT: &str = "element";
pub const GROUP: &str = "group";
pub const SIMPLE_TYPE: &str = "simpleType";

pub enum Entry {
    Annotation(annotation::Annotation),
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    SimpleType,
}

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
pub enum EntryType {
    Annotation,
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    SimpleType,
}

impl Display for EntryType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            EntryType::Annotation => { ANNOTATION }
            EntryType::AttributeGroup => { ATTRIBUTE_GROUP }
            EntryType::ComplexType => { COMPLEX_TYPE }
            EntryType::Element => { ELEMENT }
            EntryType::Group => { GROUP }
            EntryType::SimpleType => { SIMPLE_TYPE }
        };
        write!(f, "{}", s)
    }
}

impl TryInto<EntryType> for String {
    type Error = crate::error::Error;

    fn try_into(self) -> Result<EntryType, Self::Error> {
        match self.as_str() {
            ANNOTATION => Ok(EntryType::Annotation),
            ATTRIBUTE_GROUP => Ok(EntryType::AttributeGroup),
            COMPLEX_TYPE => Ok(EntryType::ComplexType),
            ELEMENT => Ok(EntryType::Element),
            GROUP => Ok(EntryType::Group),
            SIMPLE_TYPE => Ok(EntryType::SimpleType),
            _ => raise!("unknown EntryType '{}'", self.as_str()),
        }
    }
}

#[derive(Debug, Eq, PartialEq, Ord)]
pub struct ID {
    pub entry_type: EntryType,
    pub name: String,
}

impl Display for ID {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} ({})", self.name, self.entry_type)
    }
}

impl PartialOrd for ID {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if self.entry_type != other.entry_type {
            return Some(self.entry_type.cmp(&other.entry_type));
        }
        Some(self.name.cmp(&other.name))
    }
}
