mod annotation;
mod import;
mod list;
mod restriction;
mod simple_type;
mod union;

use crate::error::Result;
use annotation::Annotation;
use exile::Element;
use import::Import;
use simple_type::SimpleType;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::convert::TryInto;
use std::fmt::{Display, Formatter};
use std::path::Path;

pub struct XSD {
    entries: Vec<Entry>,
}

impl XSD {
    pub fn load<P: AsRef<Path>>(filepath: P) -> Result<Self> {
        let xml_str = wrap!(
            std::fs::read_to_string(filepath.as_ref()),
            "unable to load '{}'",
            filepath.as_ref().display()
        )?;
        let doc = exile::parse(&xml_str).unwrap();
        Self::parse(doc.root())
    }

    pub fn parse(root: &exile::Element) -> Result<Self> {
        if root.name != "schema" {
            return raise!("expected the root node to be named 'schema'");
        }
        let mut entries = Vec::new();
        for (i, entry_node) in root.children().enumerate() {
            let entry = Entry::from_xml(entry_node, i as u64)?;
            entries.push(entry);
        }
        Ok(XSD { entries: entries })
    }
}

pub const ANNOTATION: &str = "annotation";
pub const ATTRIBUTE_GROUP: &str = "attributeGroup";
pub const COMPLEX_TYPE: &str = "complexType";
pub const ELEMENT: &str = "element";
pub const GROUP: &str = "group";
pub const IMPORT: &str = "import";
pub const SIMPLE_TYPE: &str = "simpleType";

// attributes
pub const NAME: &str = "name";
pub const VALUE: &str = "value";

pub enum Entry {
    Annotation(Annotation),
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    Import(Import),
    SimpleType(SimpleType),
}

impl Entry {
    pub fn from_xml(node: &Element, index: u64) -> Result<Self> {
        let n = node.name.as_str();
        let t = EntryType::parse(n)?;
        match t {
            EntryType::Annotation => Ok(Entry::Annotation(Annotation::from_xml(node, index)?)),
            EntryType::AttributeGroup => {
                // TODO - implement AttributeGroups
                panic!("'{}' is not an implemented node type", ATTRIBUTE_GROUP);
            }
            EntryType::ComplexType => {
                // TODO - implement ComplexType
                panic!("'{}' is not an implemented node type", COMPLEX_TYPE);
            }
            EntryType::Element => {
                // TODO - implement Element
                panic!("'{}' is not an implemented node type", ELEMENT);
            }
            EntryType::Group => {
                // TODO - implement Group
                panic!("'{}' is not an implemented node type", GROUP);
            }
            EntryType::Import => Ok(Entry::Import(Import::from_xml(node, index)?)),
            EntryType::SimpleType => Ok(Entry::SimpleType(SimpleType::from_xml(node, index)?)),
            EntryType::Other(s) => {
                // TODO - implement AttributeGroups
                panic!("'{}' is not an implemented node type", s.as_str());
            }
        }
    }
}

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
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
