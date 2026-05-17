use crate::model::builtin::BuiltinString;
use crate::model::symbol::Symbol;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub struct Enumeration {
    pub name: Symbol,
    pub members: Vec<Symbol>,
    pub documentation: String,
    pub default: Symbol,
    /// There are enums that are handled specially because they were part of an element that allowed
    /// for an "other" field that could hold a string. If `other_field.is_some()`, it means that the
    /// enum behaves like a variant where some value can be held in a field that might be named, for
    /// example `other`. For an example of an 'enum' like this, see the `Dynamics` element in
    /// `musicxml.xsd`, which uses empty element choices for its enum values and an
    /// `<other>whatever</other>` choice to allow for any string value.
    pub other_field: Option<OtherField>,
}

/// For cases where something akin to an enum was defined in the XSD, such that a list of possible
/// string values was given, but an 'other' field is allowed as an escape hatch. Here we define the
/// name of that 'other' field along with its string type.
#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd)]
pub struct OtherField {
    pub name: Symbol,
    pub type_: BuiltinString,
    /// A language like Rust wouldn't need this, but in, e.g. C++, an enum cannot contain a value,
    /// so the enum and the potential 'other' value need to be 'wrapped' together in some way.
    pub wrapper_class_name: Symbol,
}
