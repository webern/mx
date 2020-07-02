use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::common::DefinedBy;
use crate::xsd::constants::{ANNOTATION, ATTRIBUTE, DEFAULT, FIXED, NAME, REF, TYPE};
use crate::xsd::{use_required, EntryType, ID};

pub struct Attribute {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub defined_by: DefinedBy,
    pub required: bool,
    pub default: Option<String>,
    pub fixed: Option<String>,
}

impl Attribute {
    pub fn documentation(&self) -> String {
        if let Some(a) = &self.annotation {
            a.documentation()
        } else {
            "".to_owned()
        }
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != ATTRIBUTE {
            return raise!("expected '{}', got '{}'", ATTRIBUTE, node.name.as_str());
        }
        let name = Self::parse_name(node);
        let defined_by = Self::parse_defined_by(node)?;
        Ok(Attribute {
            id: Self::parse_id(name.as_str(), &defined_by),
            index,
            name,
            annotation: Self::parse_annotation(node, index)?,
            required: use_required(node),
            defined_by,
            default: node.attributes.map().get(DEFAULT).cloned(),
            fixed: node.attributes.map().get(FIXED).cloned(),
        })
    }

    fn parse_defined_by(node: &exile::Element) -> Result<DefinedBy> {
        if let Some(ref_) = node.attributes.map().get(REF) {
            Ok(DefinedBy::Ref(ref_.clone()))
        } else if let Some(type_) = node.attributes.map().get(TYPE) {
            Ok(DefinedBy::Type(type_.clone()))
        } else {
            raise!("could not find either '{}' or '{}' attribute", REF, TYPE)
        }
    }

    fn parse_name(node: &exile::Element) -> String {
        if let Some(name) = node.attributes.map().get(NAME) {
            name.clone()
        } else {
            "".to_owned()
        }
    }

    fn parse_id(name: &str, defined_by: &DefinedBy) -> ID {
        ID {
            entry_type: EntryType::Other(ATTRIBUTE.to_owned()),
            name: if name.is_empty() {
                defined_by.value().to_owned()
            } else {
                name.to_owned()
            },
        }
    }

    fn parse_annotation(node: &exile::Element, index: u64) -> Result<Option<Annotation>> {
        for child in node.children() {
            let t = child.name.as_str();
            match t {
                ANNOTATION => return Ok(Some(Annotation::from_xml(node, index)?)),
                _ => return raise!("unexpected node '{}'", t),
            }
        }
        Ok(None)
    }
}
