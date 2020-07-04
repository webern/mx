use crate::error::Result;
use crate::xsd::constants::{
    ANNOTATION, ATTRIBUTE_GROUP, COMPLEX_TYPE, ELEMENT, GROUP, IMPORT, SIMPLE_TYPE,
};
use std::cmp::Ordering;
use std::fmt::{Display, Formatter};

#[derive(Clone, Debug, Eq, PartialEq, Ord)]
pub struct Id {
    entry_type: RootNodeType,
    name: String,
}

impl Id {
    pub fn new(entry_type: RootNodeType, name: String) -> Self {
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
pub enum RootNodeType {
    Annotation,
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    Import,
    SimpleType,
    Other(String),
}

impl Display for RootNodeType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            RootNodeType::Annotation => ANNOTATION,
            RootNodeType::AttributeGroup => ATTRIBUTE_GROUP,
            RootNodeType::ComplexType => COMPLEX_TYPE,
            RootNodeType::Element => ELEMENT,
            RootNodeType::Group => GROUP,
            RootNodeType::Import => IMPORT,
            RootNodeType::SimpleType => SIMPLE_TYPE,
            RootNodeType::Other(s) => s.as_str(),
        };
        write!(f, "{}", s)
    }
}

impl RootNodeType {
    pub fn parse<S: AsRef<str>>(s: S) -> Result<RootNodeType> {
        let et = match s.as_ref() {
            ANNOTATION => RootNodeType::Annotation,
            ATTRIBUTE_GROUP => RootNodeType::AttributeGroup,
            COMPLEX_TYPE => RootNodeType::ComplexType,
            ELEMENT => RootNodeType::Element,
            GROUP => RootNodeType::Group,
            IMPORT => RootNodeType::Import,
            SIMPLE_TYPE => RootNodeType::SimpleType,
            _ => RootNodeType::Other(s.as_ref().into()),
        };
        Ok(et)
    }
}

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct RootNodeId {
    index: u64,
    type_: RootNodeType,
    name: String,
}

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct ChildNodeId {
    type_: String,
    name: Option<String>,
    parent: Id,
}
