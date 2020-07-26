mod enumeration;
mod scalar_string;

use crate::model::create::{Create, CreateError, CreateResult};
use crate::model::default_create::enumeration::{is_enumeration, model_enumeration};
use crate::model::default_create::scalar_string::model_scalar_string;
use crate::model::enumeration::Enumeration;
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::{simple_type, Entry, Xsd};
use std::borrow::Borrow;
use std::ops::Deref;

#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub struct DefaultCreate {}

impl Default for DefaultCreate {
    fn default() -> Self {
        Self {}
    }
}

impl Create for DefaultCreate {
    fn name(&self) -> &'static str {
        "default"
    }

    fn create(&self, entry: &Entry, xsd: &Xsd) -> CreateResult {
        match entry {
            Entry::Annotation(_) => Ok(Some(Vec::new())), // TODO - implement Annotation
            Entry::AttributeGroup(_) => Ok(Some(Vec::new())), // TODO - implement AttributeGroup
            Entry::ComplexType(_) => Ok(Some(Vec::new())), // TODO - implement ComplexType
            Entry::Element(_) => Ok(Some(Vec::new())),    // TODO - implement Element
            Entry::Group(_) => Ok(Some(Vec::new())),      // TODO - implement Group
            Entry::Import(_) => Ok(Some(Vec::new())),     // TODO - implement Import
            Entry::SimpleType(st) => dispatch_simple_type(st, xsd),
        }
    }
}

fn dispatch_simple_type(st: &SimpleType, xsd: &Xsd) -> CreateResult {
    if is_enumeration(st) {
        model_enumeration(st, xsd)
    } else if let Some(scalar_string_result) = model_scalar_string(st, xsd) {
        scalar_string_result
    } else {
        Ok(Some(Vec::new())) // TODO - implement other simple types
    }
}
