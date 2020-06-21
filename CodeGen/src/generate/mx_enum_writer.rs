use super::musicxml_xsd::Enumeration;
use crate::generate::string_stuff::{camel_case, pascal_case, sep, Altered, Symbol};
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

pub(crate) fn write_tabs<W: Write>(w: &mut W, num: u32) -> std::io::Result<()> {
    for _ in 0..num {
        write!(w, "    ")?;
    }
    Ok(())
}

macro_rules! w {
    ($w:expr, $tabs:expr, $($arg:expr),+) => {{
        crate::generate::mx_enum_writer::write_tabs($w, $tabs)?;
        write!($w, $($arg),+)
    }};
}

macro_rules! l {
    ($w:expr, $tabs:expr, $($arg:expr),+) => {{
        crate::generate::mx_enum_writer::write_tabs($w, $tabs)?;
        writeln!($w, $($arg),+)
    }};
}

/// This represents an enum where an extra enumeration is added, e.g. `other`, and a wrapper class
/// is created allowing any arbitrary string to be held when e.g. `other` is the enum value.
#[derive(Clone)]
pub(crate) struct MxEnumOption {
    pub(crate) other_field_name: Symbol,
    pub(crate) wrapper_class_name: Symbol,
    pub(crate) default_value: Symbol,
}

#[derive(Clone)]
pub(crate) struct MxEnum {
    pub(crate) id: String,
    pub(crate) index: usize,
    pub(crate) camel_case: Symbol,
    pub(crate) pascal_case: Symbol,
    pub(crate) members: Vec<Symbol>,
    /// These are enums that I handled specially because they were part of an element that allowed
    /// for an "other" field that could hold a string. These Enums require custom handling.
    pub(crate) other_field: Option<MxEnumOption>,
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
                value: "emptystring".to_owned(),
                original: u,
            }),
            Symbol::Altered(a) => Symbol::Altered(Altered {
                value: "emptystring".to_owned(),
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
        match params.member_substitutions.get(s.value()) {
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
        l!(w, 2, "enum class {}", n)?;
        l!(w, 2, "{{")?;
        for (i, m) in self.members.iter().enumerate() {
            let is_last = i == self.members.len() - 1;
            w!(w, 3, "{} = {}", m.value(), i)?;
            if is_last {
                if let Some(other_field) = &self.other_field {
                    w!(w, 0, ",\n")?;
                    w!(w, 3, "{} = {}", other_field.other_field_name.value(), i + 1)?;
                    w!(w, 0, "\n")?;
                } else {
                    w!(w, 0, "\n")?;
                }
            } else {
                w!(w, 0, ",\n")?;
            }
        }
        l!(w, 2, "}};")?;
        l!(w, 0, "")?;
        l!(w, 2, "{} parse{}( const std::string& value );", n, n)?;
        l!(w, 2, "std::string toString( const {} value );", n)?;
        l!(
            w,
            2,
            "std::ostream& toStream( std::ostream& os, const {} value );",
            n
        )?;
        l!(
            w,
            2,
            "std::ostream& operator<<( std::ostream& os, const {} value );",
            n
        )?;

        if let Some(other_field) = &self.other_field {
            let cn = other_field.wrapper_class_name.value();
            let en = self.pascal_case.value();
            l!(w, 0, "")?;
            l!(w, 2, "class {}", cn)?;
            l!(w, 2, "{{")?;
            l!(w, 2, "public:")?;
            l!(w, 3, "explicit {}( const {} value );", cn, en)?;
            l!(w, 3, "explicit {}( const std::string& value );", cn)?;
            l!(w, 3, "{}();", cn)?;
            l!(w, 3, "{} getValue() const;", en)?;
            l!(w, 3, "std::string getValueString() const;")?;
            l!(w, 3, "void setValue( const {} value );", en)?;
            l!(w, 3, "void setValue( const std::string& value );")?;
            l!(w, 2, "private:")?;
            l!(w, 3, "{} myEnum;", en)?;
            l!(w, 3, "std::string myCustomValue;")?;
            l!(w, 2, "}};")?;
            l!(w, 0, "")?;
            l!(w, 2, "{} parse{}( const std::string& value );", cn, cn)?;
            l!(w, 2, "std::string toString( const {}& value );", cn)?;
            l!(
                w,
                2,
                "std::ostream& toStream( std::ostream& os, const {}& value );",
                cn
            )?;
            l!(
                w,
                2,
                "std::ostream& operator<<( std::ostream& os, const {}& value );",
                cn
            )?;
        }

        Ok(())
    }

    pub(crate) fn write_definition<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        if self.other_field.is_some() {
            self.write_mx_option_definition(w)
        } else {
            self.write_standard_definition(w)
        }
    }

    fn write_standard_definition<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        let pc = self.pascal_case.value();
        let cc = self.camel_case.value();
        if self.other_field.is_some() {
            panic!("bug");
        }
        l!(w, 2, "{}", sep(pc, 2))?;
        l!(w, 0, "")?;
        l!(w, 2, "{} parse{}( const std::string& value )", pc, pc)?;
        l!(w, 2, "{{")?;
        for (i, member) in self.members.iter().enumerate() {
            let o = member.original();
            let n = member.value();
            if i == 0 {
                l!(
                    w,
                    3,
                    "if ( value == \"{}\" ) {{ return {}::{}; }}",
                    o,
                    pc,
                    n
                )?;
            } else {
                l!(
                    w,
                    3,
                    "else if ( value == \"{}\" ) {{ return {}::{}; }}",
                    o,
                    pc,
                    n
                )?;
            }
        }
        l!(
            w,
            3,
            "return {}::{};",
            pc,
            self.members.get(0).unwrap().value()
        )?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "std::string toString( const {} value )", pc)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "switch ( value )")?;
        l!(w, 3, "{{")?;
        for (i, member) in self.members.iter().enumerate() {
            let o = member.original();
            let n = member.value();
            l!(w, 4, "case {}::{}: {{ return \"{}\"; }}", pc, n, o)?;
        }
        l!(w, 4, "default: break;")?;
        l!(w, 3, "}}")?;
        l!(
            w,
            3,
            "return \"{}\";",
            self.members.get(0).unwrap().original()
        )?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(
            w,
            2,
            "std::ostream& toStream( std::ostream& os, const {} value )",
            pc
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return os << toString( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(
            w,
            2,
            "std::ostream& operator<<( std::ostream& os, const {} value )",
            pc
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return toStream( os, value );")?;
        l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "{}::{}( const {} value )", cn, cn, pc)?;
        // l!(w, 2, ":myEnum( value )")?;
        // l!(w, 2, ",myCustomValue( \"\" )")?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "setValue( value );")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "{}::{}( const std::string& value )", cn, cn)?;
        // l!(w, 2, ":myEnum( {}::{} )", pc, of_pasc)?;
        // l!(w, 2, ",myCustomValue( value )")?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "setValue( value );")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "{}::{}()", cn, cn)?;
        // l!(w, 2, ":myEnum( {}::{} )", pc, other.default_value.value())?;
        // l!(w, 2, ",myCustomValue( \"\" )")?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "setValue( {}::{} );", pc, other.default_value.value())?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "{} {}::getValue() const", pc, cn)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "return myEnum;")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "std::string {}::getValueString() const", cn)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "if ( myEnum != {}::{} )", pc, of_pasc)?;
        // l!(w, 3, "{{")?;
        // l!(w, 4, "return toString( myEnum );")?;
        // l!(w, 3, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 3, "else")?;
        // l!(w, 3, "{{")?;
        // l!(w, 4, "return myCustomValue;")?;
        // l!(w, 3, "}}")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "void {}::setValue( const {} value )", cn, pc)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "myEnum = value;")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "void {}::setValue( const std::string& value )", cn)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "bool found = false;")?;
        // l!(w, 3, "{} temp = parse{}( value, found );", pc, pc)?;
        // l!(w, 3, "if ( found )")?;
        // l!(w, 3, "{{")?;
        // l!(w, 4, "myEnum = temp;")?;
        // l!(w, 3, "}}")?;
        // l!(w, 3, "else")?;
        // l!(w, 3, "{{")?;
        // l!(w, 4, "setValue( {}::{} );", pc, of_pasc)?;
        // l!(w, 4, "myCustomValue = value;")?;
        // l!(w, 3, "}}")?;
        // l!(w, 2, "}}")?;
        // l!(w, 0, "")?;
        // l!(w, 2, "{} parse{}( const std::string& value )", cn, cn)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "return {}( value );", cn)?;
        // l!(w, 2, "}}")?;
        // l!(w, 2, "")?;
        // l!(w, 2, "std::string toString( const {}& value )", cn)?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "return value.getValueString();")?;
        // l!(w, 2, "}}")?;
        // l!(w, 2, "")?;
        // l!(
        //     w,
        //     2,
        //     "std::ostream& toStream( std::ostream& os, const {}& value )",
        //     cn
        // )?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "return os << toString( value );")?;
        // l!(w, 2, "}}")?;
        // l!(w, 2, "")?;
        // l!(
        //     w,
        //     2,
        //     "std::ostream& operator<<( std::ostream& os, const {}& value )",
        //     cn
        // )?;
        // l!(w, 2, "{{")?;
        // l!(w, 3, "return toStream( os, value );")?;
        // l!(w, 2, "}}")?;
        Ok(())
    }

    fn write_mx_option_definition<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        let pc = self.pascal_case.value();
        let cc = self.camel_case.value();
        let other = if let Some(other_field) = &self.other_field {
            other_field
        } else {
            panic!("bug");
        };
        let of_orig = other.other_field_name.original();
        let of_pasc = other.other_field_name.value();
        let cn = other.wrapper_class_name.value();
        l!(w, 2, "{}", sep(pc, 2))?;
        l!(w, 0, "")?;
        l!(
            w,
            2,
            "{} parse{}( const std::string& value, bool& success )",
            pc,
            pc
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "success = true;")?;
        for (i, member) in self.members.iter().enumerate() {
            let o = member.original();
            let n = member.value();
            if i == 0 {
                l!(
                    w,
                    3,
                    "if ( value == \"{}\" ) {{ return {}::{}; }}",
                    o,
                    pc,
                    n
                )?;
            } else {
                l!(
                    w,
                    3,
                    "else if ( value == \"{}\" ) {{ return {}::{}; }}",
                    o,
                    pc,
                    n
                )?;
            }
        }
        l!(
            w,
            3,
            "else if ( value == \"{}\" ) {{ return {}::{}; }}",
            of_orig,
            pc,
            of_pasc
        )?;
        l!(w, 3, "success = false;")?;
        l!(w, 3, "return {}::{};", pc, of_pasc)?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{} parse{}( const std::string& value )", pc, pc)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "bool success = true;")?;
        l!(w, 3, "return parse{}( value, success );", pc)?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "std::string toString( const {} value )", pc)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "switch ( value )")?;
        l!(w, 3, "{{")?;
        for (i, member) in self.members.iter().enumerate() {
            let o = member.original();
            let n = member.value();
            l!(w, 4, "case {}::{}: {{ return \"{}\"; }}", pc, n, o)?;
        }
        l!(
            w,
            4,
            "case {}::{}: {{ return \"{}\"; }}",
            pc,
            of_pasc,
            of_orig
        )?;
        l!(w, 4, "default: break;")?;
        l!(w, 3, "}}")?;
        l!(w, 3, "return \"default\";")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(
            w,
            2,
            "std::ostream& toStream( std::ostream& os, const {} value )",
            pc
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return os << toString( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(
            w,
            2,
            "std::ostream& operator<<( std::ostream& os, const {} value )",
            pc
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return toStream( os, value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{}::{}( const {} value )", cn, cn, pc)?;
        l!(w, 2, ":myEnum( value )")?;
        l!(w, 2, ",myCustomValue( \"\" )")?;
        l!(w, 2, "{{")?;
        l!(w, 3, "setValue( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{}::{}( const std::string& value )", cn, cn)?;
        l!(w, 2, ":myEnum( {}::{} )", pc, of_pasc)?;
        l!(w, 2, ",myCustomValue( value )")?;
        l!(w, 2, "{{")?;
        l!(w, 3, "setValue( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{}::{}()", cn, cn)?;
        l!(w, 2, ":myEnum( {}::{} )", pc, other.default_value.value())?;
        l!(w, 2, ",myCustomValue( \"\" )")?;
        l!(w, 2, "{{")?;
        l!(w, 3, "setValue( {}::{} );", pc, other.default_value.value())?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{} {}::getValue() const", pc, cn)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return myEnum;")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "std::string {}::getValueString() const", cn)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "if ( myEnum != {}::{} )", pc, of_pasc)?;
        l!(w, 3, "{{")?;
        l!(w, 4, "return toString( myEnum );")?;
        l!(w, 3, "}}")?;
        l!(w, 0, "")?;
        l!(w, 3, "else")?;
        l!(w, 3, "{{")?;
        l!(w, 4, "return myCustomValue;")?;
        l!(w, 3, "}}")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "void {}::setValue( const {} value )", cn, pc)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "myEnum = value;")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "void {}::setValue( const std::string& value )", cn)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "bool found = false;")?;
        l!(w, 3, "{} temp = parse{}( value, found );", pc, pc)?;
        l!(w, 3, "if ( found )")?;
        l!(w, 3, "{{")?;
        l!(w, 4, "myEnum = temp;")?;
        l!(w, 3, "}}")?;
        l!(w, 3, "else")?;
        l!(w, 3, "{{")?;
        l!(w, 4, "setValue( {}::{} );", pc, of_pasc)?;
        l!(w, 4, "myCustomValue = value;")?;
        l!(w, 3, "}}")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{} parse{}( const std::string& value )", cn, cn)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return {}( value );", cn)?;
        l!(w, 2, "}}")?;
        l!(w, 2, "")?;
        l!(w, 2, "std::string toString( const {}& value )", cn)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return value.getValueString();")?;
        l!(w, 2, "}}")?;
        l!(w, 2, "")?;
        l!(
            w,
            2,
            "std::ostream& toStream( std::ostream& os, const {}& value )",
            cn
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return os << toString( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 2, "")?;
        l!(
            w,
            2,
            "std::ostream& operator<<( std::ostream& os, const {}& value )",
            cn
        )?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return toStream( os, value );")?;
        l!(w, 2, "}}")?;
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
                other_field: e.other_field.clone(),
            };
            mx_enums.push(mx)
        }
        mx_enums.sort_by(|a, b| {
            let astr = a.camel_case.value();
            let bstr = b.camel_case.value();
            astr.cmp(bstr)
        });
        Ok(MxEnumWriter {
            params,
            enums: mx_enums,
        })
    }

    pub(crate) fn write_declarations<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        self.write_h_file_open(w)?;
        for (i, e) in self.enums.iter().enumerate() {
            let is_last = i == self.enums.len() - 1;
            e.write_declaration(w)?;
            if !is_last {
                l!(w, 0, "")?;
            }
        }
        self.write_h_file_close(w)?;
        Ok(())
    }

    pub(crate) fn write_definitions<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        self.write_cpp_file_open(w)?;
        for (i, e) in self.enums.iter().enumerate() {
            let is_last = i == self.enums.len() - 1;
            e.write_definition(w)?;
            if !is_last {
                l!(w, 0, "")?;
            }
        }
        self.write_cpp_file_close(w)?;
        Ok(())
    }

    fn write_h_file_open<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        // write_tab!(w);
        l!(w, 0, "// MusicXML Class Library")?;
        l!(w, 0, "// Copyright (c) by Matthew James Briggs")?;
        l!(w, 0, "// Distributed under the MIT License")?;
        l!(w, 0, "")?;
        l!(w, 0, "#pragma once")?;
        l!(w, 0, "")?;
        l!(w, 0, "#include \"mx/core/EnumsBuiltin.h\"")?;
        l!(w, 0, "")?;
        l!(w, 0, "namespace mx")?;
        l!(w, 0, "{{")?;
        l!(w, 1, "namespace core")?;
        l!(w, 1, "{{")?;
        Ok(())
    }

    fn write_h_file_close<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        l!(w, 1, "}}")?;
        l!(w, 0, "}}")?;
        Ok(())
    }

    fn write_cpp_file_open<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        // write_tab!(w);
        l!(w, 0, "// MusicXML Class Library")?;
        l!(w, 0, "// Copyright (c) by Matthew James Briggs")?;
        l!(w, 0, "// Distributed under the MIT License")?;
        l!(w, 0, "")?;
        l!(w, 0, "#include \"mx/core/Enums.h\"")?;
        l!(w, 0, "")?;
        l!(w, 0, "#include <ostream>")?;
        l!(w, 0, "#include <string>")?;
        l!(w, 0, "")?;
        l!(w, 0, "namespace mx")?;
        l!(w, 0, "{{")?;
        l!(w, 1, "namespace core")?;
        l!(w, 1, "{{")?;
        Ok(())
    }

    fn write_cpp_file_close<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        l!(w, 1, "}}")?;
        l!(w, 0, "}}")?;
        Ok(())
    }
}
