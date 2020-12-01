mod enumeration;
mod scalar;

#[macro_use]
use crate::model::create::{Create, CreateError, CreateResult};

use crate::model;
use crate::model::default_create::enumeration::{is_enumeration, model_enumeration};
use crate::model::default_create::scalar::{
    model_derived_simple_type, model_scalar_number, model_scalar_string,
};
use crate::model::element::Element;
use crate::model::enumeration::Enumeration;
use crate::model::scalar::ScalarNumeric;
use crate::model::symbol::Symbol;
use crate::model::Def;
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::{simple_type, Entry, Xsd};
use log::trace;
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
            Entry::Annotation(a) => Ok(Some(Vec::new())), // ignore
            Entry::AttributeGroup(x) => Ok(Some(vec![Def::AttributeGroup(
                model::attribute_group::AttributeGroup::new(x.clone()),
            )])),
            Entry::ComplexType(x) => {
                // TODO - implement
                trace!("unimplemented: {}", x.id);
                Ok(Some(Vec::new()))
            }
            Entry::Element(eee) => Ok(Some(vec![Def::Element(Element::new(eee.clone()))])),
            Entry::Group(x) => {
                // TODO - implement
                trace!("unimplemented: {}", x.id);
                Ok(Some(Vec::new()))
            }
            Entry::Import(x) => Ok(Some(Vec::new())), // ignore
            Entry::SimpleType(st) => dispatch_simple_type(st, xsd),
        }
    }
}

fn dispatch_simple_type(st: &SimpleType, xsd: &Xsd) -> CreateResult {
    if is_enumeration(st) {
        model_enumeration(st, xsd)
    } else if let Some(result) = model_scalar_string(st, xsd) {
        result
    } else if let Some(result) = model_scalar_number(st, xsd) {
        result
    } else if let Some(result) = model_derived_simple_type(st, xsd) {
        result
    } else {
        //Err(make_create_err!("Unhandled SimpleType: '{}'", st.name))
        Ok(Some(Vec::new())) // TODO - implement other simple types
    }
}
