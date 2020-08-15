#[macro_use]
pub mod create;

pub mod builtin;
pub mod creator;
pub mod default_create;
pub mod enumeration;
pub mod post_process;
pub mod scalar;
pub mod symbol;
pub mod transform;

use crate::model::create::{Create, CreateError, CreateResult};
use crate::model::enumeration::Enumeration;
use crate::model::scalar::{DerivedSimpleTypeData, ScalarNumeric, ScalarString, UnionData};
use crate::model::symbol::Symbol;
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::{simple_type, Entry, Xsd};
pub use default_create::DefaultCreate;
use std::borrow::Borrow;
use std::ops::Deref;
use std::collections::HashMap;
use crate::xsd::primitives::BaseType;
use crate::error::Result;

#[derive(Debug, Clone, PartialEq, PartialOrd)]
pub enum Def {
    Enumeration(Enumeration),
    ScalarString(ScalarString),
    ScalarNumber(ScalarNumeric),
    CustomScalarString(ScalarString),
    DerivedSimpleType(DerivedSimpleTypeData),
    UnionSimpleType(UnionData),
}

impl Def {
    pub fn name(&self) -> &Symbol {
        match self {
            Def::Enumeration(x) => &x.name,
            Def::ScalarString(x) => {&x.name},
            Def::ScalarNumber(x) => {x.name()},
            Def::CustomScalarString(x) => {&x.name},
            Def::DerivedSimpleType(x) => {&x.name},
            Def::UnionSimpleType(x) => {&x.name},
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd, Eq, Ord, Hash)]
pub enum Shape {
    Simple,
    Complex,
    // TODO - not sure
    // AttributeGroup,
    // Element
}

impl Default for Shape {
    fn default() -> Self {
        Shape::Simple
    }
}

#[derive(Debug, Clone, PartialEq, PartialOrd, Eq, Ord, Hash, Default)]
pub struct Ipseity {
    shape: Shape,
    name: String,
}

#[derive(Debug, Clone, Default)]
pub struct Model {
    map: HashMap<Ipseity, Def>,
}

impl Model {
    pub fn add(&mut self, def: Def) -> Result<()> {
        let shape = match def {
            Def::Enumeration(_) => Shape::Simple,
            Def::ScalarString(_) => Shape::Simple,
            Def::ScalarNumber(_) => Shape::Simple,
            Def::CustomScalarString(_) => Shape::Simple,
            Def::DerivedSimpleType(_) => Shape::Simple,
            Def::UnionSimpleType(_) => Shape::Simple,
        };
        let ipseity = Ipseity{ shape, name: def.name().original().into() };
        match self.map.insert(ipseity.clone(), def){
            None => Ok(()),
            Some(_) => raise!("duplicate item {:?}", ipseity),
        }
    }

    pub fn get_ips(&self, ipseity: &Ipseity) -> Option<&Def>{
        self.map.get(ipseity)
    }

    pub fn get<S: AsRef<str>>(&self, shape: Shape, name: S) -> Option<&Def> {
        let ips = Ipseity{shape,name:name.as_ref().into()};
        self.get_ips(&ips)
    }

    pub fn defs(&self) -> impl Iterator<Item=&Def> {
        self.map.values()
    }

    pub fn iter(&self) -> impl Iterator<Item=(&Ipseity,&Def)> {
        self.map.iter()
    }
}