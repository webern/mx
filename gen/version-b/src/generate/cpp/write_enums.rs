use crate::error::Result;
use crate::generate::cpp::cpp_template::{render_core_cpp, render_core_h};
use crate::generate::cpp::writer::Writer;
use crate::generate::template::{render, ENUM_CPP, ENUM_H, ENUM_WRAPPER_CPP, ENUM_WRAPPER_H};
use crate::model::enumeration::{Enumeration, OtherField};
use crate::model::Def;
use crate::utils::string_stuff::{
    camel_case, documentation, linestart, pascal_case, sep, write_documentation, Altered, Symbol,
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
    pub(crate) fn write_enums(&self, enumerations: &mut [&Enumeration]) -> Result<()> {
        enumerations.sort_by(|&a, &b| {
            let a = a.name.pascal();
            let b = b.name.pascal();
            a.cmp(b)
        });
        let mut contents_h = String::new();
        let mut contents_cpp = String::new();
        for (i, enumeration) in enumerations.iter().enumerate() {
            let enumeration = *enumeration;
            let first = i == 0;
            let mut data = HashMap::new();
            data.insert("classname", enumeration.name.pascal().to_owned());
            data.insert("banner", sep(enumeration.name.pascal(), 2));
            data.insert(
                "documentation",
                documentation(&enumeration.documentation, 2)?,
            );
            data.insert("enum_members_declare", enum_members_declare(enumeration));
            data.insert("enum_members_parse", enum_members_parse(enumeration));
            data.insert(
                "enum_members_to_string",
                enum_members_to_string(enumeration),
            );
            if let Some(of) = &enumeration.other_field {
                data.insert("default_value_enum", enumeration.default.camel().into());
                data.insert(
                    "default_value_string",
                    enumeration.default.original().into(),
                );
                data.insert("to_string_default_return", of.name.original().into());
                data.insert("other_field_name", of.name.camel().into());
            } else {
                let first_member = enumeration.members.first().unwrap();
                data.insert("default_value_enum", enumeration.default.camel().into());
                data.insert(
                    "default_value_string",
                    enumeration.default.original().into(),
                );
                data.insert(
                    "to_string_default_return",
                    enumeration.default.original().into(),
                );
            }
            let rendered_h = render(ENUM_H, &data)?;
            if !first {
                contents_h.push('\n');
                contents_h.push('\n');
            }
            contents_h.push_str(&rendered_h);
            let rendered_cpp = render(ENUM_CPP, &data)?;
            if !first {
                contents_cpp.push('\n');
                contents_cpp.push('\n');
            }
            contents_cpp.push_str(&rendered_cpp);
            if let Some(of) = &enumeration.other_field {
                data.insert("wrapper_classname", of.wrapper_class_name.pascal().into());
                contents_h.push_str("\n\n");
                contents_cpp.push_str("\n\n");
                contents_h.push_str(render(ENUM_WRAPPER_H, &data)?.as_str());
                contents_cpp.push_str(render(ENUM_WRAPPER_CPP, &data)?.as_str());
            }
        }
        let file_h = render_core_h(
            contents_h,
            Some(&mut ["mx/core/EnumsBuiltin.h"]),
            Some(&mut ["iostream", "string", "optional"]),
        )?;
        let file_cpp = render_core_cpp(
            contents_cpp,
            Some("mx/core/Enums.h"),
            None,
            Some(&mut ["sstream"]),
        )?;
        wrap!(std::fs::write(&self.paths.enums_h, file_h))?;
        wrap!(std::fs::write(&self.paths.enums_cpp, file_cpp))?;
        Ok(())
    }
}

fn enum_members_declare(e: &Enumeration) -> String {
    let mut s = String::new();
    for (i, m) in e.members.iter().enumerate() {
        s.push_str(format!("            {} = {}", m.camel(), i).as_str());
        if i < e.members.len() - 1 || e.other_field.is_some() {
            s.push(',');
            s.push('\n');
        }
    }
    if let Some(of) = &e.other_field {
        s.push_str(format!("            {} = {}", of.name.camel(), e.members.len()).as_str());
    }
    s
}

fn enum_members_parse(e: &Enumeration) -> String {
    let mut s = String::new();
    for (i, m) in e.members.iter().enumerate() {
        let first = i == 0;
        if !first {
            s.push('\n');
        }
        s.push_str("            ");
        if !first {
            s.push_str("else ");
        }
        s.push_str(
            format!(
                "if( value == \"{}\" ) {{ return {}::{}; }}",
                m.original(),
                e.name.pascal(),
                m.camel()
            )
            .as_str(),
        );
    }
    if let Some(of) = &e.other_field {
        s.push_str("\n            else ");
        s.push_str(
            format!(
                "if( value == \"{}\" ) {{ return {}::{}; }}",
                of.name.original(),
                e.name.pascal(),
                of.name.camel()
            )
            .as_str(),
        );
    }
    s
}

fn enum_members_to_string(e: &Enumeration) -> String {
    let mut s = String::new();
    s.push_str("            switch ( value )\n");
    s.push_str("            {\n");
    for (i, m) in e.members.iter().enumerate() {
        s.push_str("                ");
        s.push_str(
            format!(
                "case {}::{}: {{ return \"{}\"; }}\n",
                e.name.pascal(),
                m.camel(),
                m.original()
            )
            .as_str(),
        );
    }
    s.push_str("                default: break;\n");
    s.push_str("            }");
    s
}
