use std::fs::{read_to_string, remove_file, File, OpenOptions};
use std::io::Write;
use std::path::{Path, PathBuf};
use std::str::FromStr;

use structopt::StructOpt;

use crate::error::{Error, Result};
use crate::xsd::{Base, SimpleDerivation, SimpleType};

mod compile_mx;
mod musicxml_xsd;
mod musicxml_xsd_constants;
mod musicxml_xsd_parser;

use crate::generate::musicxml_xsd_parser::parse_musicxml_xsd;
use compile_mx::compile_mx;
use musicxml_xsd::{Enumeration, TypeDefinition};
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
        println!("doing the generate thing");
        println!("{}", self.xsd.display());
        let xsd = read_to_string(&self.xsd).unwrap();
        let doc = exile::parse(xsd.as_str()).unwrap();
        let xsd = parse_musicxml_xsd(&doc).unwrap();
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
        opt.write_enums(&xsd.enumerations())?;
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

macro_rules! map (
    { $($key:expr => $value:expr),+ } => {
        {
            let mut m = ::std::collections::
            $(
                m.insert($key, $value);
            )+
            m
        }
     };
);

impl CppOptions {
    fn write_enums(&self, enumerations: &[Enumeration]) -> Result<()> {
        let mut substitutions = HashMap::new();
        substitutions.insert("16th".to_string(), "sixteenth".to_string());
        substitutions.insert("32nd".to_string(), "thirtySecond".to_string());
        substitutions.insert("64th".to_string(), "sixtyFourth".to_string());
        substitutions.insert("128th".to_string(), "oneHundredTwentyEighth".to_string());
        substitutions.insert("256th".to_string(), "twoHundredFifthySixth".to_string());
        substitutions.insert("512th".to_string(), "fiveHundredTwelfth".to_string());
        substitutions.insert("1024th".to_string(), "oneThousandTwentyFourth".to_string());
        let sanitizer = StringSanitizer {
            keywords: vec!["double", "short", "long", "continue", "do", "explicit"],
            enum_substitutions: substitutions,
            enum_suffixed: vec![
                "Step",
                "ArrowDirection",
                "ArrowStyle",
                "CircularArrow",
                "Syllabic",
                "BarStyle",
                "TimeRelation",
            ],
        };
        self.write_enums_h(enumerations, &sanitizer)?;
        self.write_enums_cpp(enumerations, &sanitizer)?;
        Ok(())
    }

    fn write_enums_h(&self, enums: &[Enumeration], sanitizer: &StringSanitizer) -> Result<()> {
        let mut h = self.open_mx_core_file("Enums.h");
        self.write_enum_h_begin(&mut h)?;
        for (i, simple_type) in enums.iter().enumerate() {
            self.write_enum_declaration(simple_type, &mut h, &sanitizer)?;
            if i < enums.len() - 1 {
                writeln!(h, "").unwrap();
            }
        }
        self.write_enum_h_end(&mut h)
    }

    fn write_enums_cpp(&self, enums: &[Enumeration], sanitizer: &StringSanitizer) -> Result<()> {
        let mut cpp = self.open_mx_core_file("Enums.cpp");
        self.write_enum_cpp_begin(&mut cpp)?;
        self.write_enum_cpp_end(&mut cpp)
    }

    fn write_enum_declaration(
        &self,
        enumeration: &Enumeration,
        h: &mut File,
        sanitizer: &StringSanitizer,
    ) -> Result<()> {
        // en = enum_name, given a short name because rustfmt is way too aggressive with line breaks
        let en = sanitizer.pascal_case(enumeration.name.as_str());
        let en = sanitizer.sanitize(en);
        let en = sanitizer.do_enum_name_suffix(en);
        writeln!(h, "        enum class {}", en).unwrap();
        writeln!(h, "        {{").unwrap();
        for (i, enval) in enumeration.members.iter().enumerate() {
            let enval = sanitizer.camel_calse(enval);
            let enval = sanitizer.sanitize(enval);
            write!(h, "             {} = {}", enval, i).unwrap();
            if i < enumeration.members.len() - 1 {
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
        writeln!(f, "#include \"mx/core/EnumsBuiltin.h\"").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "namespace mx").unwrap();
        writeln!(f, "{{").unwrap();
        writeln!(f, "    namespace core").unwrap();
        writeln!(f, "    {{").unwrap();
        Ok(())
    }

    fn write_enum_cpp_begin(&self, f: &mut File) -> Result<()> {
        writeln!(f, "// MusicXML Class Library").unwrap();
        writeln!(f, "// Copyright (c) by Matthew James Briggs").unwrap();
        writeln!(f, "// Distributed under the MIT License").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "#include \"mx/core/Enums.h\"").unwrap();
        writeln!(f, "").unwrap();
        writeln!(f, "namespace mx").unwrap();
        writeln!(f, "{{").unwrap();
        writeln!(f, "    namespace core").unwrap();
        writeln!(f, "    {{").unwrap();
        Ok(())
    }
    fn write_enum_h_end(&self, f: &mut File) -> Result<()> {
        writeln!(f, "    }}").unwrap();
        writeln!(f, "}}").unwrap();
        Ok(())
    }
    fn write_enum_cpp_end(&self, f: &mut File) -> Result<()> {
        writeln!(f, "    }}").unwrap();
        writeln!(f, "}}").unwrap();
        Ok(())
    }
}

pub(crate) struct StringSanitizer {
    pub(crate) keywords: Vec<&'static str>,

    /// These are enum value strings that are substituted arbitrarily. This was used to spell out
    /// enum values that started with numbers. For example, the pair `"16th" => "sixteenth"` ensures
    /// that the `16th` is not used as an enum value (which would be illegal.
    pub(crate) enum_substitutions: HashMap<String, String>,

    /// These are enum class names that I decided to suffix with the word `Enum` for various.
    /// So, for example, if this vector contains `"Display"` then the enum will be declared as
    /// `enum class DisplayEnum`.
    pub(crate) enum_suffixed: Vec<&'static str>,
}

impl StringSanitizer {
    pub(crate) fn sanitize<S: AsRef<str>>(&self, s: S) -> String {
        self.do_substitution(self.suffix_if_keyword(replace_if_empty(s)))
    }

    pub(crate) fn suffix_if_keyword<S>(&self, name: S) -> String
    where
        S: AsRef<str>,
    {
        let name = name.as_ref();
        for &keyword in &self.keywords {
            if keyword == name {
                return format!("{}_", name);
            }
        }
        name.into()
    }

    pub(crate) fn pascal_case<S: AsRef<str>>(&self, s: S) -> String {
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

    pub(crate) fn do_substitution<S: AsRef<str>>(&self, name: S) -> String {
        if let Some(substitute) = self.enum_substitutions.get(name.as_ref()) {
            return substitute.clone();
        }
        name.as_ref().into()
    }

    pub(crate) fn camel_calse<S: AsRef<str>>(&self, s: S) -> String {
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
                let uc = c.to_lowercase().next().unwrap();
                out.push(c);
            }
            upper = false;
        }

        // StepEnum is coming out wrong, don't know why.
        if out.len() == 1 {
            return out.to_lowercase();
        }

        out
    }

    pub(crate) fn do_enum_name_suffix<S: AsRef<str>>(&self, name: S) -> String {
        for &s in &self.enum_suffixed {
            if s == name.as_ref() {
                return format!("{}Enum", name.as_ref());
            }
        }
        name.as_ref().to_string()
    }
}

pub(crate) fn replace_if_empty<S: AsRef<str>>(s: S) -> String {
    if s.as_ref().is_empty() {
        return "emptystring".into();
    }
    s.as_ref().into()
}
