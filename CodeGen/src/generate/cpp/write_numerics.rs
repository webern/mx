use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::model::scalar::{Bound, NumericData};
use crate::utils::string_stuff::write_documentation;
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
        crate::generate::cpp::write_numerics::write_tabs($w, $tabs)?;
        write!($w, $($arg),+)
    }};
}

macro_rules! l {
    ($w:expr, $tabs:expr, $($arg:expr),+) => {{
        crate::generate::cpp::write_numerics::write_tabs($w, $tabs)?;
        writeln!($w, $($arg),+)
    }};
}

impl Writer {
    pub(crate) fn write_integers(&self, numerics: &mut [&NumericData<i64>]) -> Result<()> {
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
        wrap!(self.close_file(&mut hwrite))?;
        wrap!(self.close_file(&mut cwrite))?;
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
        l!(&mut f, 0, "#include <ostream>")?;
        l!(&mut f, 0, "#include <string>")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "namespace mx")?;
        l!(&mut f, 0, "{{")?;
        l!(&mut f, 1, "namespace core")?;
        l!(&mut f, 1, "{{")?;
        l!(
            &mut f,
            2,
            "/* alias for the int size used by this library */"
        )?;
        l!(&mut f, 2, "using IntType = int;");
        l!(&mut f, 0, "")?;
        Ok(f)
    }

    fn open_integers_cpp(&self) -> std::io::Result<impl Write> {
        let p = &self.paths.integers_cpp;
        let _igore_error = std::fs::remove_file(p);
        let mut f = OpenOptions::new().write(true).create(true).open(p)?;
        l!(&mut f, 0, "// MusicXML Class Library")?;
        l!(&mut f, 0, "// Copyright (c) by Matthew James Briggs")?;
        l!(&mut f, 0, "// Distributed under the MIT License")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "#include \"mx/core/Integers.h\"")?;
        l!(&mut f, 0, "#include <sstream>")?;
        l!(&mut f, 0, "")?;
        l!(&mut f, 0, "namespace mx")?;
        l!(&mut f, 0, "{{")?;
        l!(&mut f, 1, "namespace core")?;
        l!(&mut f, 1, "{{")?;
        Ok(f)
    }

    fn write_integer_h<W: Write>(
        &self,
        numeric: &NumericData<i64>,
        w: &mut W,
    ) -> std::io::Result<()> {
        let c = numeric.name.pascal();
        write_documentation(w, numeric.documentation.as_str(), 2)?;
        l!(w, 0, "")?;
        let range = format!(
            "Minimum: {}, Maximum: {}",
            match &numeric.range.min {
                None => "Unbounded".to_owned(),
                Some(x) => format!("{:?}", x),
            },
            match &numeric.range.max {
                None => "Unbounded".to_owned(),
                Some(x) => format!("{:?}", x),
            }
        );
        write_documentation(w, range.as_str(), 2)?;
        l!(w, 0, "")?;
        l!(w, 2, "class {}", c)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "public:")?;
        l!(w, 3, "explicit {}( IntType value );", c)?;
        l!(w, 3, "{}();", c)?;
        l!(w, 3, "IntType getValue() const;")?;
        l!(w, 3, "virtual void setValue( IntType value );")?;
        l!(w, 3, "virtual void parse( const std::string& value );")?;
        l!(w, 2, "private:")?;
        l!(w, 3, "IntType myValue;")?;
        l!(w, 2, "}};")?;
        l!(w, 0, "")?;
        l!(w, 2, "std::string toString( const {}& value );", c)?;
        l!(
            w,
            2,
            "std::ostream& toStream( std::ostream& os, const {}& value );",
            c
        )?;
        l!(
            w,
            2,
            "std::ostream& operator<<( std::ostream& os, const {}& value );",
            c
        )?;

        Ok(())
    }

    fn write_integer_cpp<W: Write>(
        &self,
        numeric: &NumericData<i64>,
        w: &mut W,
    ) -> std::io::Result<()> {
        Ok(())
    }

    fn close_file<W: Write>(&self, w: &mut W) -> std::io::Result<()> {
        l!(w, 1, "}}")?;
        l!(w, 0, "}}")?;
        Ok(())
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    pub(crate) fn write_decimals(&self, numerics: &mut [&NumericData<f64>]) -> Result<()> {
        Ok(())
    }
}
