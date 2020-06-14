use std::fs::{read_to_string, remove_file, File, OpenOptions};
use std::io::Write;
use std::path::{Path, PathBuf};
use std::str::FromStr;

use structopt::StructOpt;

use crate::error::{Error, Result};
use crate::xsd::{Base, SimpleDerivation, SimpleType};

mod compile_mx;

use compile_mx::compile_mx;

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
        println!("doing the generate thing");
        println!("{}", self.xsd.display());
        let xsd = read_to_string(&self.xsd).unwrap();
        let doc = exile::parse(xsd.as_str()).unwrap();
        let opt = if let Language::Cpp(o) = &self.language {
            o
        } else {
            panic!("unsupported output language");
        };
        println!("{}", opt.mx_repo.display());
        let re_serialized = doc.to_string();
        // std::fs::write(opt.outdir.join("out.xml"), re_serialized.as_bytes()).unwrap();
        let names = find_element_names(doc.root());
        for (i, name) in names.iter().enumerate() {
            println!("element-{}: {}", i + 1, name);
        }
        let xsd_tags = find_xsd_tags(doc.root());
        for (i, name) in xsd_tags.iter().enumerate() {
            println!("xsdtag-{}: {}", i + 1, name);
        }
        find_simple_type_restriction_bases(doc.root(), true);
        let simple_types = parse_simple_types(doc.root(), true).unwrap();
        opt.write_enums(&simple_types)?;
        // wrap!(compile_mx(&opt.mx_repo))?;
        Ok(())
    }
}

fn find_element_names(root: &exile::Element) -> Vec<String> {
    let mut set = std::collections::HashSet::new();
    walk_elements(root, &mut set);
    let mut names = Vec::new();
    for name in set {
        names.push(name.clone())
    }
    names.sort();
    names
}

fn walk_elements(current: &exile::Element, set: &mut std::collections::HashSet<String>) {
    if current.name.as_str() == "element" {
        let name = current.attributes.map().get("name").unwrap();
        set.insert(name.into());
    }
    for child_element in current.children() {
        walk_elements(child_element, set);
    }
}

fn find_xsd_tags(root: &exile::Element) -> Vec<String> {
    let mut set = std::collections::HashSet::new();
    walk_xsd_tags(root, &mut set);
    let mut names = Vec::new();
    for name in set {
        names.push(name.clone())
    }
    names.sort();
    names
}

fn walk_xsd_tags(current: &exile::Element, set: &mut std::collections::HashSet<String>) {
    set.insert(current.name.clone());
    for child_element in current.children() {
        walk_xsd_tags(child_element, set);
    }
}

fn find_simple_type_restriction_bases(root: &exile::Element, print: bool) -> Vec<Base> {
    let mut set = std::collections::HashSet::new();
    walk_simple_type_restriction_bases(root, &mut set);
    let mut bases = Vec::new();
    for name in set {
        bases.push(name.clone())
    }
    bases.sort();
    if print {
        sep();
        for base in &bases {
            println!("simpleType restriction base: {}", base);
        }
    }
    bases
}

fn walk_simple_type_restriction_bases(
    current: &exile::Element,
    set: &mut std::collections::HashSet<Base>,
) {
    if current.name == "simpleType" {
        for child_element in current.children() {
            if child_element.name == "restriction" {
                if let Some(base_str) = child_element.attributes.map().get("base") {
                    if let Ok(base) = Base::parse(base_str) {
                        // let base = Base::parse(base).unwrap();
                        set.insert(base);
                    } else {
                        panic!("could not parse '{}'", base_str);
                    }
                } else {
                    panic!("could not find base attribute");
                }
            }
        }
    }
    for child_element in current.children() {
        walk_simple_type_restriction_bases(child_element, set);
    }
}

fn parse_simple_types(root: &exile::Element, print: bool) -> Result<Vec<SimpleType>> {
    let mut set = Vec::new();
    walk_parse_simple_types(root, &mut set)?;
    let mut bases = Vec::new();
    for name in set {
        bases.push(name.clone())
    }
    bases.sort();
    if print {
        sep();
        for base in &bases {
            println!("simpleType restriction base: {:?}", base);
        }
    }
    Ok(bases)
}

fn walk_parse_simple_types(current: &exile::Element, set: &mut Vec<SimpleType>) -> Result<()> {
    if current.name == "simpleType" {
        let simpl_type = SimpleType::parse(current)?;
        set.push(simpl_type);
        Ok(())
    } else {
        for child_element in current.children() {
            walk_parse_simple_types(child_element, set)?;
        }
        Ok(())
    }
}

fn sep() {
    println!(
        "------------------------------------------------------------------------------------"
    );
}

impl CppOptions {
    fn write_enums(&self, simple_types: &Vec<SimpleType>) -> Result<()> {
        let mut h = self.open_mx_core_file("Enums.new.h");
        let mut cpp = self.open_mx_core_file("Enums.new.cpp");
        self.write_enum_h_begin(&mut h)?;
        self.write_enum_cpp_begin(&mut cpp)?;
        for (i, simple_type) in simple_types.iter().enumerate() {
            self.write_enum(simple_type, &mut cpp, &mut h)?;
            if i < simple_types.len() - 1 {
                writeln!(h, "").unwrap();
            }
        }
        self.write_enum_h_end(&mut h)?;
        self.write_enum_cpp_end(&mut cpp)?;
        Ok(())
    }

    fn write_enum(&self, simple_type: &SimpleType, cpp: &mut File, h: &mut File) -> Result<()> {
        let restriction =
            if let SimpleDerivation::Restriction(restriction) = &simple_type.derivation {
                restriction
            } else {
                return Ok(());
            };
        if restriction.enumerations.is_empty() {
            return Ok(());
        }
        let en = pascal_case(simple_type.name.as_str());
        writeln!(h, "        enum class {}", en).unwrap();
        writeln!(h, "        {{").unwrap();
        for (i, enval) in restriction.enumerations.iter().enumerate() {
            let enval = camel_case_cpp(enval);
            write!(h, "             {} = {}", enval, i).unwrap();
            if i < restriction.enumerations.len() - 1 {
                write!(h, ",").unwrap();
            }
            write!(h, "\n").unwrap();
        }
        writeln!(h, "        }};").unwrap();
        writeln!(h, "").unwrap();
        let sp = "        ";
        writeln!(h, "{}{} parse{}( const std::string& value );", sp, en, en).unwrap();
        writeln!(h, "{}std::string toString( const {} value );", sp, en).unwrap();
        writeln!(
            h,
            "{}std::ostream& toStream( std::ostream& os, const {} value );",
            sp, en
        )
        .unwrap();
        writeln!(
            h,
            "{}std::ostream& operator<<( std::ostream& os, const {} value );",
            sp, en
        )
        .unwrap();
        Ok(())
    }

    fn write_enum_h_begin(&self, f: &mut File) -> Result<()> {
        writeln!(f, "// MusicXML Class Library").unwrap();
        writeln!(f, "// Copyright (c) by Matthew James Briggs").unwrap();
        writeln!(f, "// Distributed under the MIT License").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "#pragma once").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "#include <iostream>").unwrap();
        writeln!(f, "#include <string>").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "namespace mx").unwrap();
        writeln!(f, "{{").unwrap();
        writeln!(f, "    namespace core").unwrap();
        writeln!(f, "    {{").unwrap();
        Ok(())
    }

    fn write_enum_cpp_begin(&self, f: &mut File) -> Result<()> {
        Ok(())
    }
    fn write_enum_h_end(&self, f: &mut File) -> Result<()> {
        writeln!(f, "    }}").unwrap();
        writeln!(f, "}}").unwrap();
        Ok(())
    }
    fn write_enum_cpp_end(&self, f: &mut File) -> Result<()> {
        Ok(())
    }
}

pub(crate) fn pascal_case<S: AsRef<str>>(s: S) -> String {
    let mut out = String::new();
    let mut upper = true;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            out.push(c);
        }
        upper = false;
    }
    out
}

pub(crate) fn camel_case_cpp<S: AsRef<str>>(s: S) -> String {
    let mut out = String::new();
    let mut upper = false;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            out.push(c);
        }
        upper = false;
    }
    out
}
