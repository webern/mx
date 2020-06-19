use super::musicxml_xsd::Enumeration;
use indexmap::set::IndexSet;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::path::PathBuf;

pub(crate) enum Error {
    BadThingsHappened,
}

#[derive(Debug, Clone, Eq, PartialEq, Ord, Hash, Default)]
pub struct Altered {
    value: String,
    original: String,
}

impl Display for Altered {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}({})", self.value, self.original)
    }
}

impl PartialOrd for Altered {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.value.cmp(&other.value))
    }
}

#[derive(Debug, Clone, Eq, PartialEq, Ord, Hash)]
pub enum Symbol {
    Unaltered(String),
    Altered(Altered),
}

impl Symbol {
    pub fn get(&self) -> String {
        match self {
            Symbol::Unaltered(s) => s.clone(),
            Symbol::Altered(a) => a.value.clone(),
        }
    }
}

impl PartialOrd for Symbol {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        let a = match self {
            Symbol::Unaltered(s) => s,
            Symbol::Altered(x) => &x.value,
        };
        let b = match other {
            Symbol::Unaltered(s) => s,
            Symbol::Altered(x) => &x.value,
        };
        Some(a.cmp(b))
    }
}

pub(crate) struct MxEnum {
    pub(crate) id: String,
    pub(crate) index: usize,
    pub(crate) camel_case: Symbol,
    pub(crate) pascal_case: Symbol,
    pub(crate) name: Symbol,
    pub(crate) members: Vec<Symbol>,
    /// These are enums that I handled specially because they were part of an element that allowed
    /// for an "other" field that could hold a string. These Enums require custom handling.
    pub(crate) is_algebraic: bool,
}

// impl Default for MxEnum {
//     fn default() -> Self {
//         Self {
//             id: "".to_string(),
//             index: 0,
//             camel_case: Symbol::Unaltered("".to_string()),
//             pascal_case: Symbol::Unaltered("".to_string()),
//             name: Symbol::Unaltered("".to_string()),
//             members: vec![Symbol::Unaltered("".to_string())],
//             is_algebraic: false,
//         }
//     }
// }

impl MxEnum {
    fn camel_case(e: &Enumeration, p: &MxEnumWriterParams) -> Result<Symbol, Error> {
        Err(Error::BadThingsHappened)
    }

    fn pascal_case(e: &Enumeration, p: &MxEnumWriterParams) -> Result<Symbol, Error> {
        Err(Error::BadThingsHappened)
    }

    fn name(e: &Enumeration, p: &MxEnumWriterParams) -> Result<Symbol, Error> {
        Err(Error::BadThingsHappened)
    }

    fn members(e: &Enumeration, p: &MxEnumWriterParams) -> Result<Vec<Symbol>, Error> {
        Err(Error::BadThingsHappened)
    }

    fn is_algebraic(e: &Enumeration, p: &MxEnumWriterParams) -> Result<bool, Error> {
        Err(Error::BadThingsHappened)
    }
}

pub(crate) struct MxEnumWriterParams {
    pub(crate) enumerations: Vec<Enumeration>,
    pub(crate) member_substitutions: HashMap<String, String>,
    pub(crate) suffixed_enum_names: IndexSet<String>,
    pub(crate) keywords: IndexSet<String>,
    pub(crate) enums_h: PathBuf,
    pub(crate) enums_cpp: PathBuf,
}

pub(crate) struct MxEnumWriter {
    enums: Vec<MxEnum>,
    params: MxEnumWriterParams,
}

impl MxEnumWriter {
    pub(crate) fn new(params: MxEnumWriterParams) -> Result<Self, Error> {
        let mut mx_enums = Vec::new();
        for e in &params.enumerations {
            let mx = MxEnum {
                id: e.id.clone(),
                index: e.index,
                camel_case: MxEnum::camel_case(e, &params)?,
                pascal_case: MxEnum::pascal_case(e, &params)?,
                name: MxEnum::name(e, &params)?,
                members: MxEnum::members(e, &params)?,
                is_algebraic: MxEnum::is_algebraic(e, &params)?,
            };
            mx_enums.push(mx)
        }
        Ok(MxEnumWriter {
            params,
            enums: mx_enums,
        })
    }
}
