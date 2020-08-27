use crate::model::symbol::Symbol;

#[derive(Clone, Debug, Default)]
pub struct Model {
    fake: Symbol,
}

impl Model {
    fn name(&self, id: &str) -> &Symbol {
        &self.fake
    }
}

pub struct Element<'a> {
    id: String,
    model: &'a Model,
}

impl Element<'_> {
    pub fn name(&self, id: &str) -> &Symbol {
        self.model.name(id)
    }
}

/// # Terminology
///
/// Model: The collection of all type definitions.
/// TypeDefinition: A discrete, resolved type, defined by interpreting the XSD.
/// Primitive: The base types in XSD, e.g. xs:string, xs:int, etc.
/// Scalar: A type that has no composition, e.g. an enum, a number, a string.
/// Composed: A type that consists of more than one scalar or composed sub-components.
/// ParseCheck: A restriction on the string that can be parsed into a given scalar type.
/// TypeShape? Scalar|Composed
mod nothing {}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum TypeFamily {
    /// A simple value, for example a string, number, or an enum.
    Scalar,
    /// A collection of named, scalar values. For example, an AttributeGroup.
    Structure,
    /// A structure whose members can be scalars, other structures, or nested structures. For
    /// example, an xs:element may be defined with some attributes defined inline (scalars), and
    /// some attributes defined as attribute groups (structures, or nested structures). In practice,
    /// all of an element's attributes can be described by one Structure or NestedStructure.
    NestedStructure,
    /// A sum type, for example defined by xs:choice.
    Variant,
    /// A collection of named types, of any TypeFamily, each of which can be optional (e.g.
    /// minOccurs=0, maxOccurs=1), required (e.g. minOccurs=1 maxOccurs=1), or numerous (e.g.
    /// minOccurs=N and maxOccurs=M where M is greater than 1 and can be infinity/unbounded).
    /// Additionally, an `Element` can optionally have either a `Structure` or a `NestedStructure`
    /// that is designated as its attributes.
    Element,
}

impl Default for TypeFamily {
    fn default() -> Self {
        TypeFamily::Scalar
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct ID {}

enum IDInner {
    Named(String, String),
    Generated(String),
}
