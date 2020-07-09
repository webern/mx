use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::model::enumeration::Enumeration;
use crate::model::Model;
use crate::utils::string_stuff::{
    camel_case, linestart, pascal_case, sep, write_documentation, Altered, Symbol,
};
use indexmap::set::IndexSet;
use std::collections::HashMap;
use std::fs::OpenOptions;
use std::io::Write;

pub fn write_tabs<W: Write>(w: &mut W, num: u32) -> std::io::Result<()> {
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

impl Writer {
    pub(crate) fn write_enums(&self) -> Result<()> {
        let enumerations = self.collect_enums()?;
        let mut hwrite = wrap!(self.open_enums_h())?;
        let mut cwrite = wrap!(self.open_enums_cpp())?;
        for (i, &enumer) in enumerations.iter().enumerate() {
            let is_last = i == enumerations.len() - 1;
            wrap!(self.write_enum_h(enumer, &mut hwrite))?;
            if !is_last {
                wrap!(writeln!(hwrite, ""))?;
            }
            wrap!(self.write_enum_cpp(enumer, &mut cwrite))?;
            if !is_last {
                wrap!(writeln!(cwrite, ""))?;
            }
        }
        wrap!(self.close_enums_h(&mut hwrite))?;
        wrap!(self.close_enums_cpp(&mut cwrite))?;
        Ok(())
    }

    fn collect_enums(&self) -> Result<Vec<&Enumeration>> {
        let mut result = Vec::new();
        for model in &self.models {
            if let Model::Enumeration(enumer) = model {
                result.push(enumer);
            }
        }
        result.sort_by(|a, b| a.name.camel().cmp(b.name.camel()));
        Ok(result)
    }

    fn open_enums_h(&self) -> std::io::Result<impl Write> {
        let _igore_error = std::fs::remove_file(&self.paths.enums_h);
        let mut f = OpenOptions::new()
            .write(true)
            .create(true)
            .open(&self.paths.enums_h)?;
        l!(&mut f, 0, "// MusicXML Class Library")?;
        l!(&mut f, 0, "// Copyright (c) by Matthew James Briggs")?;
        l!(&mut f, 0, "// Distributed under the MIT License")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "#pragma once")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "#include \"mx/core/EnumsBuiltin.h\"")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "namespace mx")?;
        l!(&mut f, 0, "{{")?;
        l!(&mut f, 1, "namespace core")?;
        l!(&mut f, 1, "{{")?;
        Ok(f)
    }

    fn open_enums_cpp(&self) -> std::io::Result<impl Write> {
        let _igore_error = std::fs::remove_file(&self.paths.enums_cpp);
        let mut f = OpenOptions::new()
            .write(true)
            .create(true)
            .open(&self.paths.enums_cpp)?;
        l!(&mut f, 0, "// MusicXML Class Library")?;
        l!(&mut f, 0, "// Copyright (c) by Matthew James Briggs")?;
        l!(&mut f, 0, "// Distributed under the MIT License")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "#include \"mx/core/Enums.h\"")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "#include <ostream>")?;
        l!(&mut f, 0, "#include <string>")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "namespace mx")?;
        l!(&mut f, 0, "{{")?;
        l!(&mut f, 1, "namespace core")?;
        l!(&mut f, 1, "{{")?;
        Ok(f)
    }

    fn close_enums_h<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        l!(w, 1, "}}")?;
        l!(w, 0, "}}")?;
        Ok(())
    }

    fn close_enums_cpp<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        l!(w, 1, "}}")?;
        l!(w, 0, "}}")?;
        Ok(())
    }

    fn write_enum_h<W: Write>(&self, enumer: &Enumeration, w: &mut W) -> std::io::Result<()> {
        let n = enumer.name.pascal();

        // TODO remove this debugging
        if n == "DistanceType" {
            println!("poo");
        }

        l!(w, 2, "{}", sep(n, 2))?;
        linestart(w, 2, false)?;
        writeln!(w)?;
        write_documentation(w, enumer.documentation.as_str(), 2)?;
        writeln!(w)?;
        linestart(w, 2, false)?;
        writeln!(w)?;
        l!(w, 2, "enum class {}", n)?;
        l!(w, 2, "{{")?;
        for (i, m) in enumer.members.iter().enumerate() {
            let is_last = i == enumer.members.len() - 1;
            w!(w, 3, "{} = {}", m.camel(), i)?;
            if is_last {
                if let Some(other_field) = &enumer.other_field {
                    w!(w, 0, ",\n")?;
                    w!(w, 3, "{} = {}", other_field.name.camel(), i + 1)?;
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

        if let Some(other_field) = &enumer.other_field {
            let cn = other_field.wrapper_class_name.pascal();
            let en = enumer.name.pascal();
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

    fn write_enum_cpp<W: Write>(&self, enumer: &Enumeration, w: &mut W) -> Result<()> {
        Ok(())
    }
}
