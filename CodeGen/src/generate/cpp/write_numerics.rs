use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::model::scalar::NumericData;
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
        crate::generate::cpp::write_enums::write_tabs($w, $tabs)?;
        write!($w, $($arg),+)
    }};
}

macro_rules! l {
    ($w:expr, $tabs:expr, $($arg:expr),+) => {{
        crate::generate::cpp::write_enums::write_tabs($w, $tabs)?;
        writeln!($w, $($arg),+)
    }};
}

impl Writer {
    pub(crate) fn write_integers(&self, numerics: &mut [&NumericData<i32>]) -> Result<impl Write> {
        numerics.sort_by(|&a, &b| {
            let a = a.name.pascal();
            let b = b.name.pascal();
            a.cmp(b)
        });
        let mut hwrite = wrap!(self.open_integers_h())?;
        let mut cwrite = wrap!(self.open_integers_cpp())?;
        for (i, &integer) in numerics.iter().enumerate() {
            let is_last = i == numerics.len() - 1;
            wrap!(self.write_integer_h(integer, &mut hwrite))?;
            if !is_last {
                wrap!(writeln!(hwrite, ""))?;
            }
            wrap!(self.write_integer_cpp(integer, &mut cwrite))?;
            if !is_last {
                wrap!(writeln!(cwrite, ""))?;
            }
        }
        wrap!(self.close_integers_h(&mut hwrite))?;
        wrap!(self.close_integers_cpp(&mut cwrite))?;
        Ok(())
    }

    fn open_integers_h(&self) -> std::io::Result<impl Write> {
        let p = &self.paths.integers_h;
        let _igore_error = std::fs::remove_file(p);
        let mut f = OpenOptions::new().write(true).create(true).open(p)?;
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

    fn open_integers_cpp(&self) -> std::io::Result<impl Write> {
        let p = &self.paths.integers_cpp;
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

    fn write_integer_h<W: Write>(
        &self,
        numeric: &NumericData<i32>,
        w: &mut W,
    ) -> std::io::Result<()> {
        Ok(())
    }

    fn write_integer_cpp<W: Write>(
        &self,
        numeric: &NumericData<i32>,
        w: &mut W,
    ) -> std::io::Result<()> {
        Ok(())
    }
}
