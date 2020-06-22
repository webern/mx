use crate::error::Result;
use std::fs::{read_to_string};

mod compile_mx;
mod musicxml_xsd;
mod musicxml_xsd_constants;
mod musicxml_xsd_parser;
mod mx_enum_writer;
mod mx_writer;
mod string_stuff;
mod paths;

use crate::generate::musicxml_xsd_constants::{pseudo_enums, reserved_words, suffixed_enum_names};
use crate::generate::musicxml_xsd_parser::{parse_musicxml_xsd, XsdParserParams};
use crate::generate::mx_enum_writer::{MxEnumWriter, MxEnumWriterParams};
use crate::generate::mx_writer::MxWriter;
use musicxml_xsd_constants::enum_member_substitutions;
use crate::generate::paths::Paths;

pub struct GenArgs {
    pub paths: Paths,
}

impl Default for GenArgs {
    fn default() -> Self {
        Self {
            paths: Paths::default(),
        }
    }
}

/// Generate `mx::core` in C++
pub fn run(args: GenArgs) -> Result<()> {
    println!("{}", args.paths.xsd_3_0.display());
    let xsd = read_to_string(&args.paths.xsd_3_0).unwrap();
    let doc = exile::parse(xsd.as_str()).unwrap();
    let xsd = parse_musicxml_xsd(
        &doc,
        XsdParserParams {
            pseudo_enums: pseudo_enums(),
        },
    ).unwrap();

    let enum_params = MxEnumWriterParams {
        enumerations: xsd.enumerations(),
        member_substitutions: enum_member_substitutions(),
        suffixed_enum_names: suffixed_enum_names(),
        reserved_words: reserved_words(),
    };
    let enum_writer = MxEnumWriter::new(enum_params).unwrap();
    let writer = MxWriter::new(enum_writer);
    {
        let mut h = args.paths.create_core_file("Enums.h");
        writer.write_enum_declarations(&mut h).unwrap();
    }
    {
        let mut cpp = args.paths.create_core_file("Enums.cpp");
        writer.write_enum_definitions(&mut cpp).unwrap();
    }
    Ok(())
}
