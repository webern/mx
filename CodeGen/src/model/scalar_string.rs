use crate::model::builtin::BuiltinString;
use crate::model::symbol::Symbol;
use crate::xsd::primitives::Character;
use std::num::NonZeroU64;

#[derive(Debug, Clone, Eq, PartialEq, Ord, PartialOrd, Default)]
pub struct ScalarString {
    pub name: Symbol,
    pub base_type: Character,
    pub documentation: String,
    pub min_length: Option<u64>,
    pub max_length: Option<u64>,
    pub pattern: Option<String>,
}
