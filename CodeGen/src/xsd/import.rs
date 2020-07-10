use crate::error::Result;

use crate::xsd::annotation::Annotation;

use crate::xsd::constants::{ANNOTATION, IMPORT, SCHEMA_LOCATION};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::{namespace_attribute, Xsd};

#[derive(Clone, Debug)]
pub struct Import {
    pub id: Id,
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        if node.name.as_str() != IMPORT {
            return raise!("expected '{}', got '{}'", IMPORT, &node.name);
        }
        let (id, lineage) = Id::make(lineage, node)?;
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
                annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?);
                break;
            }
        }
        Ok(Import {
            id,
            annotation,
            namespace,
            schema_location,
        })
    }
}
