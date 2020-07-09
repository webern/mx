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
        let mut cwrite = self.open_enums_cpp()?;
        for enumer in &enumerations {
            wrap!(self.write_enum_h(enumer, &mut hwrite))?;
            wrap!(self.write_enum_cpp(enumer, &mut cwrite))?;
        }
        self.close_enums_h(&mut hwrite)?;
        self.close_enums_cpp(&mut cwrite)?;
        Ok(())
    }

    fn collect_enums(&self) -> Result<Vec<&Enumeration>> {
        let mut result = Vec::new();
        for model in &self.models {
            if let Model::Enumeration(enumer) = model {
                result.push(enumer);
            }
        }
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

    fn open_enums_cpp(&self) -> Result<impl Write> {
        let _igore_error = std::fs::remove_file(&self.paths.enums_cpp);
        let mut f = wrap!(OpenOptions::new()
            .write(true)
            .create(true)
            .open(&self.paths.enums_cpp))?;
        Ok(f)
    }

    fn close_enums_h<W: Write>(&self, w: W) -> Result<()> {
        unimplemented!();
    }

    fn close_enums_cpp<W: Write>(&self, w: W) -> Result<()> {
        unimplemented!();
    }

    fn write_enum_h<W: Write>(&self, enumer: &Enumeration, w: &mut W) -> std::io::Result<()> {
        Ok(())
    }

    fn write_enum_cpp<W: Write>(&self, enumer: &Enumeration, w: &mut W) -> Result<()> {
        Ok(())
    }
}
