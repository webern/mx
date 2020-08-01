use crate::error::Result;
use crate::generate::cpp::writer::Writer;
use crate::generate::template::{render, INTEGER_BUILTINS_CPP, INTEGER_BUILTINS_H, CORE_H, INTEGER_TYPE_H};
use crate::model::scalar::{Bound, NumericData, Range};
use crate::model::scalar::{ScalarNumeric, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::utils::string_stuff::{sep, write_documentation, documentation};
use crate::xsd::primitives::BaseType;
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::Primitive;
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
        numerics.push(byte);
        numerics.push(positive_integer);
        numerics.push(non_negative_integer);
        numerics.sort_by(|a, b| {
            let a = a.name.pascal();
            let b = b.name.pascal();
            a.cmp(b)
        });
        self.write_integers_h(numerics.as_slice())?;
        self.write_integers_cpp(&numerics)?;
        Ok(())
    }

    fn write_integers_h(&self, numerics: &[NumericData<i64>]) -> Result<()> {
        let mut contents = String::new();
        let mut nothing = HashMap::<String,String>::new();

        let builtins = render(INTEGER_BUILTINS_H, &nothing)?;
        contents.push_str(&builtins);
        for numeric in numerics {
            // // TODO - remove
            // if numeric.name.original() == "positiveInteger" || numeric.name.original() == "nonNegativeInteger" {
            //     continue;
            // }
            let mut data = HashMap::new();
            data.insert("classname", numeric.name.pascal().to_owned());
            data.insert("documentation", documentation(&numeric.documentation, 2)?);
            let rendered_type = render(INTEGER_TYPE_H, &data)?;
            contents.push('\n');
            contents.push('\n');
            contents.push_str(&rendered_type);
        }
        let mut final_data = HashMap::new();
        final_data.insert("contents", contents);
        final_data.insert("std_includes", r#"

#include <iostream>
#include <limits>
#include <string>"#.to_owned());
        let final_contents = render(CORE_H, &final_data)?;
        wrap!(std::fs::write(&self.paths.integers_h, final_contents))?;
        Ok(())
    }

    fn write_integers_cpp(&self, numerics: &[NumericData<i64>]) -> Result<()> {
        // let nothing = HashMap::<String,String>::new();
        // let builtins = render(INTEGER_BUILTINS_CPP, &nothing)?;
        // let mut rendered_types = Vec::new();
        // for numeric in numerics {
        //     // TODO - remove
        //     if numeric.name.original() == "positiveInteger" || numeric.name.original() == "nonNegativeInteger" {
        //         continue;
        //     }
        //     let rendered_type = render(INTEGER_BUILTINS_CPP, &nothing)?;
        //     rendered_types.push(rendered_type);
        // }
        Ok(())
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    pub(crate) fn write_decimals(&self, numerics: &mut [&NumericData<f64>]) -> Result<()> {
        Ok(())
    }
}
