use std::fs::{read_to_string, remove_file, File, OpenOptions};
use std::path::{Path, PathBuf};
use std::str::FromStr;

use structopt::StructOpt;

use crate::error::{Error, Result};
use crate::xsd::{Base, SimpleDerivation, SimpleType};

mod compile_mx;
mod musicxml_xsd;
mod musicxml_xsd_constants;
mod musicxml_xsd_parser;
mod mx_enum_writer;
mod mx_writer;
mod string_stuff;

use crate::generate::musicxml_xsd_constants::{pseudo_enums, reserved_words, suffixed_enum_names};
use crate::generate::musicxml_xsd_parser::{parse_musicxml_xsd, XsdParserParams};
use crate::generate::mx_enum_writer::{MxEnumWriter, MxEnumWriterParams};
use crate::generate::mx_writer::MxWriter;
use compile_mx::compile_mx;
use musicxml_xsd::{Enumeration, TypeDefinition};
use musicxml_xsd_constants::enum_member_substitutions;
use std::collections::HashMap;

#[derive(Debug, StructOpt)]
pub struct GenerateArgs {
    /// Path to musicxml.xsd
    #[structopt(short = "x", long = "xsd")]
    xsd: PathBuf,
    /// Path to the mx repo root
    #[structopt(short = "m", long = "mxrepo")]
    mx_repo: Option<PathBuf>,
}

fn path_to_repo() -> PathBuf {
    let mut p = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    p.pop();
    p.canonicalize().unwrap()
}

impl GenerateArgs {
    pub(crate) fn run(&self) -> Result<()> {
        println!("args: {:?}", self);
        let g = Generator::new(
            self.xsd.clone(),
            Language::Cpp(CppOptions {
                mx_repo: if let Some(r) = &self.mx_repo {
                    r.clone()
                } else {
                    path_to_repo()
                },
            }),
        );
        g.generate()
    }
}

pub enum Language {
    Rust,
    Cpp(CppOptions),
}

pub struct Generator {
    xsd: PathBuf,
    language: Language,
}

pub struct CppOptions {
    mx_repo: PathBuf,
}

impl CppOptions {
    fn path_mx_core(&self) -> PathBuf {
        self.mx_repo
            .join("Sourcecode")
            .join("private")
            .join("mx")
            .join("core")
    }

    pub(crate) fn open_mx_core_file<S: AsRef<str>>(&self, filename: S) -> File {
        let p = self.path_mx_core().join(filename.as_ref());
        if p.is_file() {
            remove_file(&p).unwrap();
        }
        // let _ = std::fs::remove(&p);
        println!("opening a file for writing: '{}'", p.display());
        open_file(p)
    }
}

pub(crate) fn open_file<P: AsRef<Path>>(p: P) -> File {
    OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(p)
        .unwrap()
}

impl Generator {
    fn new(xsd: PathBuf, language: Language) -> Generator {
        Self { xsd, language }
    }

    fn generate(&self) -> Result<()> {
        println!("{}", self.xsd.display());
        let xsd = read_to_string(&self.xsd).unwrap();
        let doc = exile::parse(xsd.as_str()).unwrap();
        let xsd = parse_musicxml_xsd(
            &doc,
            XsdParserParams {
                pseudo_enums: pseudo_enums(),
            },
        )
        .unwrap();
        let opt = if let Language::Cpp(o) = &self.language {
            o
        } else {
            panic!("unsupported output language");
        };
        println!("{}", opt.mx_repo.display());

        let enum_params = MxEnumWriterParams {
            enumerations: xsd.enumerations(),
            member_substitutions: enum_member_substitutions(),
            suffixed_enum_names: suffixed_enum_names(),
            reserved_words: reserved_words(),
            enums_h: Default::default(),
            enums_cpp: Default::default(),
        };
        let enum_writer = MxEnumWriter::new(enum_params).unwrap();
        let writer = MxWriter::new(enum_writer);
        {
            let mut h = opt.open_mx_core_file("Enums.h");
            writer.write_enum_declarations(&mut h).unwrap();
        }
        {
            let mut cpp = opt.open_mx_core_file("Enums.cpp");
            writer.write_enum_definitions(&mut cpp).unwrap();
        }
        Ok(())
    }
}
