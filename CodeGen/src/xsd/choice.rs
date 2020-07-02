use crate::error::Result;
use crate::xsd::annotation::Annotation;

use crate::xsd::constants::{ANNOTATION};
use crate::xsd::{EntryType, ID};


pub struct Choice {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
}

impl Choice {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        // if node.name.as_str() != CHOICE {
        //     return raise!("expected '{}', got '{}'", CHOICE, node.name.as_str());
        // }
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        let id = ID {
            entry_type: EntryType::Element,
            name: "name_attribute(node)?".into(),
        };
        Ok(Choice {
            id,
            index,
            annotation,
        })
    }
}

#[test]
fn parse() {
    unimplemented!();
}
