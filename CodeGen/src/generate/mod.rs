extern crate env_logger;

use std::fs::read_to_string;
use std::path::PathBuf;
use std::str::FromStr;

use snafu::{Backtrace, GenerateBacktrace, ResultExt};
use structopt::StructOpt;

use crate::error::{Error, Result};
use crate::xsd::{Base, SimpleType};

#[derive(Debug, StructOpt)]
pub struct GenerateArgs {
    /// Path to musicxml.xsd
    #[structopt(short = "x", long = "xsd")]
    xsd: PathBuf,
}

impl GenerateArgs {
    pub(crate) fn run(&self) -> Result<()> {
        trace!("args: {:?}", self);
        let g = Generator::new(
            self.xsd.clone(),
            Language::Cpp(CppOptions {
                outdir: PathBuf::from_str(".").unwrap().canonicalize().unwrap(),
            }));
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
    outdir: PathBuf,
}

impl Generator {
    fn new(xsd: PathBuf, language: Language) -> Generator {
        Self {
            xsd,
            language,
        }
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
        println!("{}", opt.outdir.display());
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
        // Err(Error::CommandExec {
        //     message: "generate".to_string(),
        //     backtrace: Backtrace::generate(),
        // })
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

fn walk_simple_type_restriction_bases(current: &exile::Element, set: &mut std::collections::HashSet<Base>) {
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
    println!("------------------------------------------------------------------------------------");
}