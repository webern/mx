use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::model::enumeration::Enumeration;
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
    fn write_enums(&self) -> Result<()> {
        let enumerations = self.collect_enums()?;
        let mut hwrite = self.open_enums_h()?;
        let mut cwrite = self.open_enums_cpp()?;
        for enumer in &enumerations {
            wrap!(self.write_enum_h(enumer, &mut hwrite))?;
            wrap!(self.write_enum_cpp(enumer, &mut cwrite))?;
        }
        self.close_enums_h(&mut hwrite)?;
        self.clone_enums_cpp(&mut cwrite)?;
        Ok(())
    }

    fn collect_enums(&self) -> Result<Vec<Enumeration>> {
        Ok(Vec::new())
    }

    fn open_enums_h(&self) -> Result<impl Write> {
        let _igore_error = std::fs::remove_file(&self.paths.enums_h);
        let mut f = wrap!(OpenOptions::new()
            .write(true)
            .append(true)
            .open(&self.paths.enums_h))?;
        Ok(f)
    }

    fn open_enums_cpp(&self) -> Result<impl Write> {
        let _igore_error = std::fs::remove_file(&self.paths.enums_cpp);
        let mut f = wrap!(OpenOptions::new()
            .write(true)
            .append(true)
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
