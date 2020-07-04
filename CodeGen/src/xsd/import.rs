use crate::error::Result;

use crate::xsd::annotation::Annotation;

use crate::xsd::constants::{ANNOTATION, IMPORT, SCHEMA_LOCATION};
use crate::xsd::id::{EntryType, Id};
use crate::xsd::namespace_attribute;

#[derive(Clone, Debug)]
pub struct Import {
    pub id: Id,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub namespace: String,
    pub schema_location: String,
}

impl Import {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != IMPORT {
            return raise!("expected '{}', got '{}'", IMPORT, &node.name);
        }
        let namespace = namespace_attribute(node)?;
        let schema_location = node
            .attributes
            .map()
            .get(SCHEMA_LOCATION)
            .ok_or_else(|| make_err!("'{}' attribute not found", SCHEMA_LOCATION))?
            .clone();
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        let id = Id::new(EntryType::Other(IMPORT.to_owned()), schema_location.clone());
        Ok(Import {
            id,
            index,
            annotation,
            namespace,
            schema_location,
        })
    }
}
