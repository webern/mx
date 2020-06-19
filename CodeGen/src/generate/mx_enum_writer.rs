use super::musicxml_xsd::Enumeration;
use crate::generate::string_stuff::{camel_case, pascal_case, Altered, Symbol};
use indexmap::set::IndexSet;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::path::PathBuf;

pub(crate) enum Error {
    BadThingsHappened,
}

pub(crate) struct MxEnum {
    pub(crate) id: String,
    pub(crate) index: usize,
    pub(crate) camel_case: Symbol,
    pub(crate) pascal_case: Symbol,
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
    fn rename(s: Symbol, params: &MxEnumWriterParams) -> Symbol {
        MxEnum::add_reserved_suffix(
            MxEnum::add_enum_suffix(MxEnum::replace_word(s, &params), &params),
            &params,
        )
    }

    fn add_enum_suffix(s: Symbol, params: &MxEnumWriterParams) -> Symbol {
        if !MxEnum::is_enum_suffixed(s.value(), &params) {
            return s;
        }
        match s {
            Symbol::Unaltered(u) => Symbol::Altered(Altered {
                value: format!("{}Enum", &u),
                original: u,
            }),
            Symbol::Altered(a) => Symbol::Altered(Altered {
                value: format!("{}Enum", &a.value),
                original: a.original,
            }),
        }
    }

    fn add_reserved_suffix(s: Symbol, params: &MxEnumWriterParams) -> Symbol {
        if !MxEnum::is_reserved_word(s.value(), &params) {
            return s;
        }
        match s {
            Symbol::Unaltered(u) => Symbol::Altered(Altered {
                value: format!("{}Enum", &u),
                original: u,
            }),
            Symbol::Altered(a) => Symbol::Altered(Altered {
                value: format!("{}Enum", &a.value),
                original: a.original,
            }),
        }
    }

    fn replace_word(s: Symbol, params: &MxEnumWriterParams) -> Symbol {
        match params.member_substitutions.get(s.value().as_str()) {
            None => s,
            Some(new_name) => match s {
                Symbol::Unaltered(u) => Symbol::Altered(Altered {
                    value: new_name.clone(),
                    original: u,
                }),
                Symbol::Altered(a) => Symbol::Altered(Altered {
                    value: new_name.clone(),
                    original: a.original,
                }),
            },
        }
    }

    // fn add_reserved_word_suffix(s: Symbol, is_reserved_word: bool) -> Symbol {
    //     match s {
    //         Symbol::Unaltered(u) => {
    //             if is_enum_suffixed {
    //                 Symbol::Altered(Altered {
    //                     value: format!("{}_", &u),
    //                     original: u,
    //                 })
    //             } else {
    //                 Symbol::Unaltered(u)
    //             }
    //         }
    //         Symbol::Altered(a) => {
    //             if is_enum_suffixed {
    //                 Symbol::Altered(Altered {
    //                     value: format!("{}_", &a.value),
    //                     original: a.original,
    //                 })
    //             } else {
    //                 Symbol::Altered(a)
    //             }
    //         }
    //     }
    // }

    fn is_enum_suffixed<S: AsRef<str>>(s: S, params: &MxEnumWriterParams) -> bool {
        params.suffixed_enum_names.get(s.as_ref()) != None
    }

    fn is_reserved_word<S: AsRef<str>>(s: S, params: &MxEnumWriterParams) -> bool {
        params.reserved_words.get(s.as_ref()) != None
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
    pub(crate) reserved_words: IndexSet<String>,
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
                camel_case: MxEnum::rename(Symbol::new(&e.name), &params),
                pascal_case: MxEnum::rename(Symbol::new(&e.name), &params),
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
