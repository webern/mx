use crate::error::Result;
use crate::model::builtin::BuiltinString;
use crate::model::symbol::Symbol;
use crate::xsd::primitives::{Character, Numeric};
use std::fmt::Debug;
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

#[derive(Debug, Clone, PartialEq, PartialOrd)]
pub enum ScalarNumeric {
    Decimal(NumericData<f64>),
    Integer(NumericData<i64>),
}

impl Default for ScalarNumeric {
    fn default() -> Self {
        ScalarNumeric::Integer(NumericData::default())
    }
}

#[derive(Debug, Clone, PartialEq, PartialOrd)]
pub enum Bound<T>
where
    T: Debug + Clone + Copy + PartialEq + PartialOrd + Default,
{
    Inclusive(T),
    Exclusive(T),
}

impl<T> Default for Bound<T>
where
    T: Debug + Clone + Copy + PartialEq + PartialOrd + Default,
{
    fn default() -> Self {
        Bound::Inclusive(Default::default())
    }
}

#[derive(Debug, Clone, PartialEq, PartialOrd, Default)]
pub struct Range<T>
where
    T: Debug + Clone + Copy + PartialEq + PartialOrd + Default,
{
    pub min: Option<Bound<T>>,
    pub max: Option<Bound<T>>,
}

impl<T> Range<T>
where
    T: Debug + Clone + Copy + PartialEq + PartialOrd + Default,
{
    pub fn new(min: Option<Bound<T>>, max: Option<Bound<T>>) -> Self {
        Self { min, max }
    }
}

#[derive(Debug, Clone, PartialEq, PartialOrd, Default)]
pub struct NumericData<T>
where
    T: Debug + Clone + Copy + PartialEq + PartialOrd + Default,
{
    pub name: Symbol,
    pub base_type: Numeric,
    pub documentation: String,
    pub range: Range<T>,
}
