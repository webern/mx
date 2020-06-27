mod annotation;
mod attribute_group;
mod complex_type;
mod element;
mod group;
mod import;
mod list;
mod restriction;
mod simple_type;
mod union;

use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::attribute_group::AttributeGroup;
use crate::xsd::complex_type::ComplexType;
use crate::xsd::element::Element;
use crate::xsd::group::Group;
use crate::xsd::import::Import;
use crate::xsd::simple_type::SimpleType;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::convert::TryInto;
use std::fmt::{Display, Formatter};
use std::path::Path;

pub struct Xsd {
    entries: Vec<Entry>,
}

impl Xsd {
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
        Ok(Xsd { entries: entries })
    }
}

impl Display for Xsd {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        for entry in &self.entries {
            writeln!(f, "{}", entry.id())?;
        }
        Ok(())
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
    AttributeGroup(AttributeGroup),
    ComplexType(ComplexType),
    Element(Element),
    Group(Group),
    Import(Import),
    SimpleType(SimpleType),
}

impl Entry {
    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let n = node.name.as_str();
        let t = EntryType::parse(n)?;
        match t {
            EntryType::Annotation => Ok(Entry::Annotation(Annotation::from_xml(node, index)?)),
            EntryType::AttributeGroup => Ok(Entry::AttributeGroup(AttributeGroup::from_xml(
                node, index,
            )?)),
            EntryType::ComplexType => Ok(Entry::ComplexType(ComplexType::from_xml(node, index)?)),
            EntryType::Element => Ok(Entry::Element(Element::from_xml(node, index)?)),
            EntryType::Group => Ok(Entry::Group(Group::from_xml(node, index)?)),
            EntryType::Import => Ok(Entry::Import(Import::from_xml(node, index)?)),
            EntryType::SimpleType => Ok(Entry::SimpleType(SimpleType::from_xml(node, index)?)),
            EntryType::Other(s) => {
                // TODO - implement AttributeGroups
                panic!("'{}' is not an implemented node type", s.as_str());
            }
        }
    }

    pub fn id(&self) -> &ID {
        match self {
            Entry::Annotation(x) => &x.id,
            Entry::AttributeGroup(x) => &x.id,
            Entry::ComplexType(x) => &x.id,
            Entry::Element(x) => &x.id,
            Entry::Group(x) => &x.id,
            Entry::Import(x) => &x.id,
            Entry::SimpleType(x) => &x.id,
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

pub(crate) fn get_attribute<S: AsRef<str>>(
    node: &exile::Element,
    attribute_name: S,
) -> Result<String> {
    Ok(node
        .attributes
        .map()
        .get(attribute_name.as_ref())
        .ok_or(make_err!(
            "'{}' attribute not found",
            attribute_name.as_ref()
        ))?
        .clone())
}

pub(crate) fn name_attribute(node: &exile::Element) -> Result<String> {
    get_attribute(node, NAME)
}

pub(crate) fn value_attribute(node: &exile::Element) -> Result<String> {
    get_attribute(node, VALUE)
}
