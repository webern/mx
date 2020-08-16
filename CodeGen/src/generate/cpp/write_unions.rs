use crate::error::Result;
use crate::generate::cpp::cpp_template::{render_core_cpp, render_core_h};
use crate::generate::cpp::writer::Writer;
use crate::generate::template::{render, CORE_H, DECIMAL_BUILTINS_CPP, DECIMAL_BUILTINS_H, DECIMAL_TYPE_CPP, DECIMAL_TYPE_H, INTEGER_BUILTINS_CPP, INTEGER_BUILTINS_H, INTEGER_TYPE_CPP, INTEGER_TYPE_H, NO_DATA, UNION_H, UNION_CPP};
use crate::model::scalar::{Bound, NumericData, Range, UnionData};
use crate::model::scalar::{ScalarNumeric, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::{Def, Shape};
use crate::utils::string_stuff::{documentation, sep, write_documentation};
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::Primitive;
use crate::xsd::primitives::{BaseType, PrefixedString};
use indexmap::Equivalent;
use std::collections::HashMap;
use std::fs::{OpenOptions, write};
use std::io::Write;
use std::ops::Deref;
use indexmap::set::IndexSet;
use crate::generate::cpp::helpers::default_construct;

struct Info<'a> {
    union: &'a UnionData,
    named_members: &'a Vec<&'a Def>,
    primitive_members: &'a HashMap<String, &'a str>,
}

impl Writer {
    pub(crate) fn write_unions(&self, mut unions: &[&UnionData]) -> Result<()> {
        for &union in unions {
            let mut data = HashMap::new();
            // find dependencies
            let mut named_members = Vec::new();
            let mut primitive_members = HashMap::new();
            for member in &union.members {
                match member {
                    BaseType::Custom(name) => {
                        let found = match self.model.get(Shape::Simple, name) {
                        None => return raise!("the dependency '{}' could not be found for union '{}'", name, union.name.original()),
                        Some(f) => f,
                        };
                        named_members.push(found);
                    },
                    p => {
                        let primitive = p.primitive();
                        let primitive_name = format!("{}", Symbol::new(format!("{}", p)).pascal());
                        match primitive {
                            Primitive::None => panic!("can't happen"),
                            Primitive::Numeric => {
                                if member.is_decimal() {
                                    primitive_members.insert(primitive_name.clone(), "mx/core/Decimals.h");
                                } else {
                                    primitive_members.insert(primitive_name.clone(), "mx/core/Integers.h");
                                }
                            },
                            Primitive::Character => {let _ = primitive_members.insert(primitive_name.clone(), "mx/core/Strings.h");}
                            Primitive::DateType => {let _ = primitive_members.insert(primitive_name.clone(), "mx/core/Date.h");}
                        }
                    }
                }
            }
            let info = Info{
                union,
                named_members: &named_members,
                primitive_members: &primitive_members
            };
            data.insert("classname", union.name.pascal().into());
            data.insert("default_value", default_value(&info));
            data.insert("documentation", documentation(union.documentation.as_str(), 2)?);
            data.insert("parse_def", parse_def(&info));
            data.insert("variants_ctor_decl", variants_ctor_decl(&info));
            data.insert("variants_ctor_def", variants_ctor_def(&info));
            data.insert("variants_get_decl", variants_get_decl(&info));
            data.insert("variants_get_def", variants_get_def(&info));
            data.insert("variants_get_is_decl", variants_get_is_decl(&info));
            data.insert("variants_get_is_def", variants_get_is_def(&info));
            data.insert("variants_set_decl", variants_set_decl(&info));
            data.insert("variants_set_def", variants_set_def(&info));
            data.insert("variants_template_decl", variants_template_decl(&info));
            let hpath = self.paths.core.join(format!("{}.h",union.name.pascal()));
            let cpppath = self.paths.core.join(format!("{}.cpp",union.name.pascal()));
            let hcontents = render(UNION_H, &data)?;
            let cppcontents = render(UNION_CPP, &data)?;
            let h = render_core_h(hcontents, None, None)?;
            let self_import = format!("mx/core/{}.h", union.name.pascal());
            let cpp = render_core_cpp(cppcontents, Some(self_import), None, None)?;
            wrap!(write(hpath, h))?;
            wrap!(write(cpppath, cpp))?;
        }
        Ok(())
    }

    pub(crate) fn write_unions_old(&self, mut unions: &[&UnionData]) -> Result<()> {
        for &union in unions {
            // we check each union to make sure its members match our expectations,
            // then we copy the hand-written implementation. by checking the members
            // we ensure that we don't miss a change in musicxml.xsd
            match union.name.original() {
                "font-size" => {
                    if union.members.len() != 2 {
                        return raise!("font-size has wrong number of members: {:?}", union);
                    }
                    if !union.members.get(0).unwrap().is_decimal() {
                        return raise!("font-size expected decimal");
                    }
                    if union.members.get(1).unwrap().name() != "css-font-size" {
                        return raise!("font-size expected decimal");
                    }
                    self.write_union(union)?;
                }
                unhanded => return raise!("unhandled union: '{}'", unhanded),
            }
        }
        Ok(())
    }

    fn write_union(&self, union: &UnionData) -> Result<()> {
        let mut data = HashMap::new();
        let members = union
            .members
            .iter()
            .map(|item| format!("{}", item))
            .collect::<Vec<String>>()
            .join(", ");
        data.insert("classname", union.name.pascal());
        data.insert("documentation", union.documentation.as_str());
        data.insert("union", &members);
        let filename_h = format!("{}.h", union.name.pascal());
        let filename_cpp = format!("{}.cpp", union.name.pascal());
        let template_h = format!("{}.template", filename_h);
        let template_cpp = format!("{}.template", filename_cpp);
        let contents_h = render(template_h, &data)?;
        wrap!(std::fs::write(
            self.paths.core.join(&filename_h),
            contents_h
        ));
        let contents_cpp = render(template_cpp, &data)?;
        wrap!(std::fs::write(
            self.paths.core.join(&filename_cpp),
            contents_cpp
        ));
        Ok(())
    }
}

fn default_value(i: &Info<'_>) -> String {
    let first = i.union.members.first().unwrap();
    match first {
        BaseType::Custom(custom) => {
            for &def in i.named_members {
                return default_construct(def);
            }
            panic!("could not find the default value for a named base type");
        }
        _=>{
            for (k, _) in i.primitive_members {
                return format!("{}{{}}", k);
            }
            panic!("could not find the default primitive value");
        }
    }
}

fn parse_def(i: &Info<'_>) -> String { String::from("parse_def") }

fn variants_ctor_decl(i: &Info<'_>) -> String { String::from("variants_ctor_decl") }
fn variants_ctor_def(i: &Info<'_>) -> String { String::from("variants_ctor_def") }
fn variants_get_decl(i: &Info<'_>) -> String { String::from("variants_get_decl") }
fn variants_get_def(i: &Info<'_>) -> String { String::from("variants_get_def") }
fn variants_get_is_decl(i: &Info<'_>) -> String { String::from("variants_get_is_decl") }
fn variants_get_is_def(i: &Info<'_>) -> String { String::from("variants_get_is_def") }
fn variants_set_decl(i: &Info<'_>) -> String { String::from("variants_set_decl") }
fn variants_set_def(i: &Info<'_>) -> String { String::from("variants_set_def") }
fn variants_template_decl(i: &Info<'_>) -> String { String::from("variants_template_decl") }
