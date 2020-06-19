use super::musicxml_xsd::Enumeration;
use crate::generate::string_stuff::{camel_case, pascal_case, Altered, Symbol};
use indexmap::set::IndexSet;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::io::Write;
use std::path::PathBuf;

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
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

impl MxEnum {
    fn rename(s: Symbol, params: &MxEnumWriterParams) -> Symbol {
        let s = MxEnum::replace_empty_string(s);
        let s = MxEnum::replace_word(s, &params);
        let s = MxEnum::add_enum_suffix(s, &params);
        let s = MxEnum::add_reserved_suffix(s, &params);
        s
    }

    fn replace_empty_string(s: Symbol) -> Symbol {
        if !s.value().is_empty() {
            return s;
        }
        match s {
            Symbol::Unaltered(u) => Symbol::Altered(Altered {
                value: "emptyString".to_owned(),
                original: u,
            }),
            Symbol::Altered(a) => Symbol::Altered(Altered {
                value: "emptyString".to_owned(),
                original: a.original,
            }),
        }
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
                value: format!("{}_", &u),
                original: u,
            }),
            Symbol::Altered(a) => Symbol::Altered(Altered {
                value: format!("{}_", &a.value),
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

    fn is_enum_suffixed<S: AsRef<str>>(s: S, params: &MxEnumWriterParams) -> bool {
        for n in &params.suffixed_enum_names {
            if n.to_lowercase() == s.as_ref().to_lowercase() {
                return true;
            }
        }
        false
    }

    fn is_reserved_word<S: AsRef<str>>(s: S, params: &MxEnumWriterParams) -> bool {
        params.reserved_words.get(s.as_ref()) != None
    }

    fn members(e: &Enumeration, params: &MxEnumWriterParams) -> Result<Vec<Symbol>, Error> {
        let x = e
            .members
            .iter()
            .map(|m| MxEnum::rename(camel_case(&m), params))
            .collect();
        Ok(x)
    }

    fn is_algebraic(e: &Enumeration, p: &MxEnumWriterParams) -> Result<bool, Error> {
        Ok(false)
    }

    pub(crate) fn write_declaration<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        let n = self.pascal_case.value();
        writeln!(w, "enum class {}", n)?;
        writeln!(w, "{{")?;
        for (i, m) in self.members.iter().enumerate() {
            let is_last = i == self.members.len() - 1;
            write!(w, "{} = {}", m.value(), i)?;
            if is_last {
                write!(w, "\n")?;
            } else {
                write!(w, ",\n")?;
            }
        }
        writeln!(w, "}};")?;
        writeln!(w, "")?;
        writeln!(w, "{} parse{}( const std::string& value );", n, n)?;
        writeln!(w, "std::string toString( const {} value );", n)?;
        writeln!(
            w,
            "std::ostream& toStream( std::ostream& os, const {} value );",
            n
        )?;
        writeln!(
            w,
            "std::ostream& operator<<( std::ostream& os, const {} value );",
            n
        )?;
        Ok(())
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
                camel_case: MxEnum::rename(camel_case(&e.name), &params),
                pascal_case: MxEnum::rename(pascal_case(&e.name), &params),
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

    pub(crate) fn write_declarations<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        self.write_h_file_open(w)?;
        for e in &self.enums {
            e.write_declaration(w)?;
        }
        self.write_h_file_close(w)?;
        Ok(())
    }

    fn write_h_file_open<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        writeln!(w, "// MusicXML Class Library")?;
        writeln!(w, "// Copyright (c) by Matthew James Briggs")?;
        writeln!(w, "// Distributed under the MIT License")?;
        writeln!(w, "")?;
        writeln!(w, "#pragma once")?;
        writeln!(w, "")?;
        writeln!(w, "#include \"mx/core/EnumsBuiltin.h\"")?;
        writeln!(w, "")?;
        writeln!(w, "namespace mx")?;
        writeln!(w, "{{")?;
        writeln!(w, "    namespace core")?;
        writeln!(w, "    {{")?;
        Ok(())
    }

    fn write_h_file_close<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }
}
