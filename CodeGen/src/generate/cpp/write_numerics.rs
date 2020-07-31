use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::model::scalar::{Bound, NumericData, Range};
use crate::model::scalar::{ScalarNumeric, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::utils::string_stuff::{sep, write_documentation};
use crate::xsd::primitives::BaseType;
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::Primitive;
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
    pub(crate) fn write_integers(&self, mut numerics: Vec<NumericData<i64>>) -> Result<()> {
        // yuckily add these because i'm not dealing with XSD built-ins right now, but I used
        // PositiveInteger is referenced by, e.g. PositiveIntegerOrEmpty
        let positive_integer = NumericData {
            name: Symbol::new("positiveInteger"),
            base_type: Numeric::PositiveInteger,
            documentation: String::from("The built-in primitive xs:positiveInteger"),
            range: Range {
                min: Some(Bound::Inclusive(1 as i64)),
                max: None,
            },
        };
        let non_negative_integer = NumericData {
            name: Symbol::new("nonNegativeInteger"),
            base_type: Numeric::NonNegativeInteger,
            documentation: String::from("The built-in primitive xs:nonNegativeInteger"),
            range: Range {
                min: Some(Bound::Inclusive(0 as i64)),
                max: None,
            },
        };
        // i used an 'int range' for Color to clamp the range of the bytes from 0-255. to replace
        // that i'll create a 'Byte' numeric
        let byte = NumericData {
            name: Symbol::new("byte"),
            base_type: Numeric::NonNegativeInteger,
            documentation: String::from("This is not part of MusicXML. It represents a clamped byte."),
            range: Range {
                min: Some(Bound::Inclusive(0 as i64)),
                max: Some(Bound::Inclusive(255 as i64)),
            },
        };
        numerics.push(positive_integer);
        numerics.push(non_negative_integer);
        numerics.push(byte);
        numerics.sort_by(|a, b| {
            let a = a.name.pascal();
            let b = b.name.pascal();
            a.cmp(b)
        });
        let mut hwrite = wrap!(self.open_integers_h())?;
        let mut cwrite = wrap!(self.open_integers_cpp())?;
        for (i, integer) in numerics.iter().enumerate() {
            let is_last = i == numerics.len() - 1;
            wrap!(self.write_integer_h(&integer, &mut hwrite))?;
            if !is_last {
                wrap!(writeln!(hwrite, ""))?;
            }
            wrap!(self.write_integer_cpp(&integer, &mut cwrite))?;
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
        l!(&mut f, 2, "/// Alias for the int type we will use throughout this library.")?;
        l!(&mut f, 2, "using IntType = int;");
        l!(&mut f, 0, "")?;
        l!(&mut f, 2, "/// A base class for integer types")?;
        l!(&mut f, 2, "class Integer")?;
        l!(&mut f, 2, "{{")?;
            l!(&mut f, 3, "public:")?;
            l!(&mut f, 3, "virtual ~Integer() = default;")?;
            l!(&mut f, 3, "virtual IntType getValue() const = 0;")?;
            l!(&mut f, 3, "virtual void setValue( IntType value ) = 0;")?;
            l!(&mut f, 3, "virtual void parse( const std::string& value ) = 0;")?;
        l!(&mut f, 2, "}};")?;
        l!(&mut f, 2, "")?;
        l!(&mut f, 2, "std::string toString( const Integer& value );")?;
        l!(&mut f, 2, "std::ostream& toStream( std::ostream& os, const Integer& value );")?;
        l!(&mut f, 2, "std::ostream& operator<<( std::ostream& os, const Integer& value );")?;
        l!(&mut f, 2, "")?;
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

    fn write_integer_h<W: Write>(&self, numeric: &NumericData<i64>, w: &mut W) -> std::io::Result<()> {
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
        l!(w, 2, "class {} final : public Integer", c)?;
        l!(w, 2, "{{")?;
        l!(w, 2, "public:")?;
        l!(w, 3, "explicit {}( IntType value );", c)?;
        l!(w, 3, "{}();", c)?;
        l!(w, 3, "virtual ~{}() = default;", c)?;
        l!(w, 3, "virtual IntType getValue() const override;")?;
        l!(w, 3, "virtual void setValue( IntType value ) override;")?;
        l!(w, 3, "virtual void parse( const std::string& value ) override;")?;
        l!(w, 2, "private:")?;
        l!(w, 3, "IntType myValue;")?;
        l!(w, 2, "}};")?;
        l!(w, 0, "")?;
        Ok(())
    }

    fn write_integer_cpp<W: Write>(&self, numeric: &NumericData<i64>, w: &mut W) -> std::io::Result<()> {
        let c = numeric.name.pascal();
        l!(w, 2, "{}", sep(c, 2))?;
        l!(w, 0, "")?;
        l!(w, 2, "{}::{}()", c, c)?;
        l!(w, 2, ": {}{{ 0 }}", c)?;
        l!(w, 2, "{{")?;
        l!(w, 0, "")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "{}::{}( IntType value )", c, c)?;
        l!(w, 2, ": myValue{{}}")?;
        l!(w, 2, "{{")?;
        l!(w, 3, "setValue( value );")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "IntType {}::getValue() const", c)?;
        l!(w, 2, "{{")?;
        l!(w, 3, "return myValue;")?;
        l!(w, 2, "}}")?;
        l!(w, 0, "")?;
        l!(w, 2, "void {}::setValue( IntType value )", c)?;
        l!(w, 2, "{{")?;
        self.set_integer_value_impl(&numeric.range, w)?;
        l!(w, 2, "}}")?;
        Ok(())
    }

    fn set_integer_value_impl<W: Write>(&self, range: &Range<i64>, w: &mut W) -> std::io::Result<()> {
        if range.min.is_none() && range.max.is_none() {
            l!(w, 3, "myValue = value;")?;
            return Ok(());
        }
        w!(w, 3, "if( value")?;
        if let Some(min) = &range.min {
            let min_val = match min {
                Bound::Inclusive(x) => *x,
                Bound::Exclusive(x) => *x + 1,
            };
            write!(w, " < {} )\n", min_val)?;
            l!(w, 3, "{{")?;
            l!(w, 4, "myValue = {};", min_val)?;
            l!(w, 3, "}}")?;
        }
        if range.min.is_some() && range.max.is_some() {
            w!(w, 3, "else ")?;
        }
        if let Some(max) = &range.max {
            let max_val = match max {
                Bound::Inclusive(x) => *x,
                Bound::Exclusive(x) => *x - 1,
            };
            write!(w, "if( value > {} )\n", max_val)?;
            l!(w, 3, "{{")?;
            l!(w, 4, "myValue = {};", max_val)?;
            l!(w, 3, "}}")?;
        }
        l!(w, 3, "else")?;
        l!(w, 3, "{{")?;
        l!(w, 4, "myValue = value;")?;
        l!(w, 3, "}}")?;
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
