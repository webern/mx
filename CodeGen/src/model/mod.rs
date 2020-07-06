pub mod builtin;
pub mod enumeration;
pub mod symbol;
use crate::generate::string_stuff::tokenize;
use crate::model::enumeration::Enumeration;
use crate::model::symbol::Symbol;
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::{simple_type, Entry, Xsd};
use std::borrow::Borrow;
use std::ops::Deref;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub enum Model {
    Enumeration(Enumeration),
}

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub enum Creation {
    /// The models are intentionally being omitted, no further processing is needed.
    Omitted,
    /// The `Create` trait did not create a model, try the next `Create` object.
    NotHandled,
    /// The `Model`s are created, no further processing is needed.
    Created(Vec<Model>),
}

#[derive(Debug)]
pub enum Error {
    Message(String),
    Error(crate::error::Error),
}

impl Error {
    pub fn new<S: AsRef<str>>(message: S) -> Self {
        Error::Message(message.as_ref().into())
    }
    fn convert(self) -> crate::error::Error {
        match self {
            Error::Message(msg) => make_err!("{}", msg.as_str()),
            Error::Error(err) => err,
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

fn convert_result<T>(result: Result<T>) -> crate::error::Result<T> {
    match result {
        Ok(t) => Ok(t),
        Err(e) => Err(e.convert()),
    }
}

pub trait Create {
    fn create(entry: &Entry, xsd: &Xsd) -> Result<Creation>;
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub struct DefaultCreate {}

impl Create for DefaultCreate {
    fn create(entry: &Entry, xsd: &Xsd) -> Result<Creation> {
        match entry {
            Entry::Annotation(_) => Ok(Creation::Omitted), // TODO - implement Annotation
            Entry::AttributeGroup(_) => Ok(Creation::Omitted), // TODO - implement AttributeGroup
            Entry::ComplexType(_) => Ok(Creation::Omitted), // TODO - implement ComplexType
            Entry::Element(_) => Ok(Creation::Omitted),    // TODO - implement Element
            Entry::Group(_) => Ok(Creation::Omitted),      // TODO - implement Group
            Entry::Import(_) => Ok(Creation::Omitted),     // TODO - implement Import
            Entry::SimpleType(st) => {
                if is_enumeration(st)? {
                    model_enumeration(st, xsd)
                } else {
                    unimplemented!();
                }
            }
        }
    }
}

fn is_enumeration(st: &SimpleType) -> Result<bool> {
    match &st.payload {
        Payload::Restriction(r) => {
            for facet in &r.facets {
                match facet {
                    Facet::Enumeration(_) => continue,
                    _ => return Ok(false),
                }
            }
            return Ok(true);
        }
        _ => Ok(false),
    }
}

fn model_enumeration(st: &SimpleType, xsd: &Xsd) -> Result<Creation> {
    let restriction = if let simple_type::Payload::Restriction(r) = &st.payload {
        r
    } else {
        return Err(Error::Error(make_err!("not a restriction!")));
    };
    let mut members = Vec::new();
    for facet in &restriction.facets {
        let s = if let Facet::Enumeration(s) = facet {
            s
        } else {
            return Err(Error::Error(make_err!("not an enumeration!")));
        };
        members.push(Symbol::new(s.as_str()));
    }
    let enm = Enumeration {
        name: Symbol::new(st.id.name()),
        members,
        documentation: st.documentation(),
        other_field: None,
    };
    Ok(Creation::Created(vec![Model::Enumeration(enm)]))
}
