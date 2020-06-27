use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::{EntryType, ANNOTATION, ID, IMPORT};
use std::convert::TryInto;

pub struct Import {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub namespace: String,
    pub schema_location: String,
}

pub const NAMESPACE: &str = "namespace";
pub const SCHEMA_LOCATION: &str = "schemaLocation";

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
        let namespace = node
            .attributes
            .map()
            .get(NAMESPACE)
            .ok_or_else(|| make_err!("'{}' attribute not found", NAMESPACE))?
            .clone();
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
        let id = ID {
            entry_type: EntryType::Other(IMPORT.to_owned()),
            name: schema_location.clone(),
        };
        Ok(Import {
            id,
            index,
            annotation,
            namespace,
            schema_location,
        })
    }
}
