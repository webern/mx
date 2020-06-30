use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::attributes::AttributeGroup;
use crate::xsd::choice::Choice;
use crate::xsd::complex_content::ComplexContent;
use crate::xsd::constants::{ANNOTATION, COMPLEX_TYPE};
use crate::xsd::group::Group;
use crate::xsd::sequence::Sequence;
use crate::xsd::simple_content::SimpleContent;
use crate::xsd::{name_attribute, simple_content, EntryType, ID};
use std::convert::TryInto;

pub struct ComplexType {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

pub enum Payload {
    ComplexContent(ComplexContent),
    SimpleContent(SimpleContent),
    Parent(Parent),
}

pub enum Children {
    Choice(Choice),
    Group(Group),
    Sequence(Sequence),
}

pub struct Parent {
    attributes: Vec<AttributeGroup>,
    children: Option<Children>,
}

impl ComplexType {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != COMPLEX_TYPE {
            return raise!("expected '{}', got '{}'", COMPLEX_TYPE, node.name.as_str());
        }
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        let id = ID {
            entry_type: EntryType::ComplexType,
            name: "TODO".into(),
        };
        Ok(ComplexType {
            id,
            index,
            name: "TODO".into(),
            annotation,
            payload: Payload::Parent(Parent {
                attributes: vec![],
                children: None,
            }),
        })
    }
}

#[test]
fn parse() {
    unimplemented!();
}
