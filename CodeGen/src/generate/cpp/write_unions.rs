use crate::error::{Error, Result};
use crate::generate::cpp::cpp_template::{render_core_cpp, render_core_h};
use crate::generate::cpp::helpers::{default_construct, enum_default};
use crate::generate::cpp::writer::Writer;
use crate::generate::template::{
    render, CORE_H, DECIMAL_BUILTINS_CPP, DECIMAL_BUILTINS_H, DECIMAL_TYPE_CPP, DECIMAL_TYPE_H,
    INTEGER_BUILTINS_CPP, INTEGER_BUILTINS_H, INTEGER_TYPE_CPP, INTEGER_TYPE_H, NO_DATA,
    NUMBER_OR_NORMAL_CPP, NUMBER_OR_NORMAL_H, POSITIVE_INTEGER_OR_EMPTY_CPP,
    POSITIVE_INTEGER_OR_EMPTY_H, UNION_CPP, UNION_H,
};
use crate::model::scalar::{Bound, NumericData, Range, UnionData};
use crate::model::scalar::{ScalarNumeric, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::{Def, Shape};
use crate::utils::string_stuff::{documentation, sep, write_documentation};
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::Primitive;
use crate::xsd::primitives::{BaseType, PrefixedString};
use indexmap::set::IndexSet;
use indexmap::Equivalent;
use log::error;
use std::collections::HashMap;
use std::fs::{write, OpenOptions};
use std::io::Write;
use std::ops::Deref;

struct Info<'a> {
    union: &'a UnionData,
    named_members: &'a Vec<&'a Def>,
    primitive_members: &'a HashMap<String, &'a str>,
}
impl<'a> Info<'a> {
    fn find_def<S: AsRef<str>>(&'a self, name: S) -> Result<&'a Def> {
        for &def in self.named_members {
            if def.name().original() == name.as_ref() {
                return Ok(def);
            }
        }
        raise!("could not find '{}'", name.as_ref())
    }

    fn construction(&self, bt: &BaseType) -> Result<String> {
        // let first = self.union.members.first().ok_or_else(|| make_err!("no members"))?;
        match bt {
            BaseType::Custom(custom) => {
                for &def in self.named_members {
                    return Ok(default_construct(def));
                }
                return raise!("could not find def");
            }
            _ => {
                for (k, _) in self.primitive_members {
                    return Ok(format!("{}{{}}", k));
                }
                return raise!("could not find the default primitive value");
            }
        }
    }

    fn construct_first(&self) -> Result<String> {
        let first = self
            .union
            .members
            .first()
            .ok_or_else(|| make_err!("no members"))?;
        self.construction(first)
    }

    fn h_includes(&self) -> Vec<String> {
        let mut map = HashMap::new();
        for &def in self.named_members {
            match def {
                Def::Enumeration(e) => {
                    let _ = map.insert("mx/core/Enums.h".to_owned(), ());
                }
                Def::ScalarString(_) => {
                    let _ = map.insert("mx/core/Strings.h".to_owned(), ());
                } // TODO dumb?
                Def::ScalarNumber(n) => match n {
                    ScalarNumeric::Decimal(_) => {
                        let _ = map.insert("mx/core/Decimals.h".to_owned(), ());
                    }
                    ScalarNumeric::Integer(_) => {
                        let _ = map.insert("mx/core/Integers.h".to_owned(), ());
                    }
                },
                Def::CustomScalarString(s) => {
                    let _ = map.insert(format!("mx/core/{}.h", s.name.pascal()), ());
                }
                Def::DerivedSimpleType(s) => {
                    let _ = map.insert(format!("mx/core/{}.h", s.name.pascal()), ());
                }
                Def::UnionSimpleType(s) => {
                    let _ = map.insert(format!("mx/core/{}.h", s.name.pascal()), ());
                }
                Def::Element(element) => {
                    panic!(
                        "An element should not be encountered here: '{}'",
                        element.name().original()
                    );
                }
                Def::AttributeGroup(ag) => {
                    error!("unhandled: {}", ag.name().original())
                }
            }
        }
        for (_, &include) in self.primitive_members {
            map.insert(include.into(), ());
        }
        let mut list = Vec::new();
        for (include, _) in &map {
            list.push(include.to_owned());
        }
        list.sort();
        list
    }
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
                            None => {
                                return raise!(
                                    "the dependency '{}' could not be found for union '{}'",
                                    name,
                                    union.name.original()
                                )
                            }
                            Some(f) => f,
                        };
                        named_members.push(found);
                    }
                    p => {
                        let primitive = p.primitive();
                        let primitive_name = format!("{}", Symbol::new(format!("{}", p)).pascal());
                        match primitive {
                            Primitive::None => panic!("can't happen"),
                            Primitive::Numeric => {
                                if member.is_decimal() {
                                    primitive_members
                                        .insert(primitive_name.clone(), "mx/core/Decimals.h");
                                } else {
                                    primitive_members
                                        .insert(primitive_name.clone(), "mx/core/Integers.h");
                                }
                            }
                            Primitive::Character => {
                                let _ = primitive_members
                                    .insert(primitive_name.clone(), "mx/core/Strings.h");
                            }
                            Primitive::DateType => {
                                let _ = primitive_members
                                    .insert(primitive_name.clone(), "mx/core/Date.h");
                            }
                        }
                    }
                }
            }
            let info = Info {
                union,
                named_members: &named_members,
                primitive_members: &primitive_members,
            };
            data.insert("classname", union.name.pascal().into());
            data.insert("default_value", info.construct_first()?);
            data.insert(
                "documentation",
                documentation(union.documentation.as_str(), 2)?,
            );
            data.insert("parse_def", parse_def(&info)?);
            data.insert("variants_ctor_decl", variants_ctor_decl(&info)?);
            data.insert("variants_ctor_def", variants_ctor_def(&info)?);
            data.insert("variants_get_decl", variants_get_decl(&info)?);
            data.insert("variants_get_def", variants_get_def(&info)?);
            data.insert("variants_get_is_decl", variants_get_is_decl(&info)?);
            data.insert("variants_get_is_def", variants_get_is_def(&info)?);
            data.insert("variants_set_decl", variants_set_decl(&info)?);
            data.insert("variants_set_def", variants_set_def(&info)?);
            data.insert("variants_template_decl", variants_template_decl(&info));
            data.insert("variants_to_stream_decl", variants_to_stream_decl(&info));

            // These are funky
            if union.name.original() == "positive-integer-or-empty"
                || union.name.original() == "number-or-normal"
            {
                self.custom_unions(union, &mut data)?;
                continue;
            }

            let hpath = self.paths.core.join(format!("{}.h", union.name.pascal()));
            let cpppath = self.paths.core.join(format!("{}.cpp", union.name.pascal()));
            let hcontents = render(UNION_H, &data)?;
            let cppcontents = render(UNION_CPP, &data)?;
            let mut h_includes = info.h_includes();
            let mut h_strs: Vec<&str> = h_includes.iter().map(|s| s.as_str()).collect();
            let mut std_h = vec!["string", "variant", "ostream"];
            let h = render_core_h(
                hcontents,
                Some(h_strs.as_mut_slice()),
                Some(std_h.as_mut_slice()),
            )?;
            let self_import = format!("mx/core/{}.h", union.name.pascal());
            let mut std_cpp = vec!["sstream", "type_traits"];
            let cpp = render_core_cpp(
                cppcontents,
                Some(self_import),
                None,
                Some(std_cpp.as_mut_slice()),
            )?;
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

    fn custom_unions(&self, union: &UnionData, data: &mut HashMap<&str, String>) -> Result<()> {
        let (hfilename, cppfilename, mut includes) =
            if union.name.original() == "positive-integer-or-empty" {
                (
                    POSITIVE_INTEGER_OR_EMPTY_H,
                    POSITIVE_INTEGER_OR_EMPTY_CPP,
                    vec!["mx/core/Integers.h"],
                )
            } else if union.name.original() == "number-or-normal" {
                (
                    NUMBER_OR_NORMAL_H,
                    NUMBER_OR_NORMAL_CPP,
                    vec!["mx/core/Decimals.h"],
                )
            } else {
                return raise!("unexpected type '{}'", union.name.original());
            };
        let hstuff = render(hfilename, data)?;
        let cstuff = render(cppfilename, data)?;
        let h = render_core_h(
            hstuff,
            Some(includes.as_mut_slice()),
            Some(vec!["string", "memory", "iostream"].as_mut_slice()),
        )?;
        let self_include = format!("mx/core/{}.h", union.name.pascal());
        let mut c_include = vec!["sstream"];
        let c = render_core_cpp(
            cstuff,
            Some(&self_include),
            None,
            Some(c_include.as_mut_slice()),
        )?;
        wrap!(write(
            self.paths.core.join(format!("{}.h", union.name.pascal())),
            h
        ))?;
        wrap!(write(
            self.paths.core.join(format!("{}.cpp", union.name.pascal())),
            c
        ))?;
        Ok(())
    }
}

fn parse_def(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    for m in &i.union.members {
        if let BaseType::Custom(name) = m {
            let def = i.find_def(name)?;
            if let Def::Enumeration(enumer) = def {
                s.push_str(
                    format!(
                        "            const auto {} = tryParse{}( value );\n",
                        enumer.name.camel(),
                        enumer.name.pascal()
                    )
                    .as_str(),
                );
                s.push_str(format!("            if( {} )\n", enumer.name.camel()).as_str());
                s.push_str("            {\n");
                s.push_str(
                    format!(
                        "                set{}( *{} );\n",
                        enumer.name.pascal(),
                        enumer.name.camel()
                    )
                    .as_str(),
                );
                s.push_str("                return true;\n");
                s.push_str("            }\n");
                continue;
            } else {
                let name = Symbol::new(format!("{}", m));
                s.push_str(
                    format!(
                        "            auto {} = {}{{}};\n",
                        name.camel(),
                        name.pascal()
                    )
                    .as_str(),
                );
                s.push_str(format!("            if( {}.parse( value ) )\n", name.camel()).as_str());
                s.push_str("            {\n");
                s.push_str(
                    format!(
                        "                set{}( {} );\n",
                        name.pascal(),
                        name.camel()
                    )
                    .as_str(),
                );
                s.push_str("                return true;\n");
                s.push_str("            }\n");
            }
        } else {
            let name = Symbol::new(format!("{}", m));
            s.push_str(
                format!(
                    "            auto {} = {}{{}};\n",
                    name.camel(),
                    name.pascal()
                )
                .as_str(),
            );
            s.push_str(format!("            if( {}.parse( value ) )\n", name.camel()).as_str());
            s.push_str("            {\n");
            s.push_str(
                format!(
                    "                set{}( {} );\n",
                    name.pascal(),
                    name.camel()
                )
                .as_str(),
            );
            s.push_str("                return true;\n");
            s.push_str("            }\n");
        }
    }
    s.push_str("            return false;");
    Ok(s)
}

fn variants_ctor_decl(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        s.push_str(
            format!(
                "            explicit {}( {} value );",
                clss,
                m.to_symbol().pascal()
            )
            .as_str(),
        );
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}
fn variants_ctor_def(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        s.push_str(
            format!(
                "        {}::{}( {} value )\n",
                clss,
                clss,
                m.to_symbol().pascal()
            )
            .as_str(),
        );
        let do_move = match i.find_def(m.name()) {
            Ok(ok) => match ok {
                Def::Enumeration(_) => false,
                _ => true,
            },
            Err(_) => true,
        };
        if do_move {
            s.push_str("        : myValue{ std::move( value ) }\n");
        } else {
            s.push_str("        : myValue{ value }\n");
        }
        s.push_str("        {\n");
        s.push_str("\n");
        s.push_str("        }\n");
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}
fn variants_get_decl(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("            {} getValue{}() const;", pascal, pascal).as_str());
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}

fn variants_get_def(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("        {} {}::getValue{}() const\n", pascal, clss, pascal).as_str());
        s.push_str("        {\n");
        s.push_str(format!("            auto result = {};\n", i.construction(m)?).as_str());
        s.push_str("            std::visit([&](auto&& arg)\n");
        s.push_str("            {\n");
        s.push_str("                using T = std::decay_t<decltype(arg)>;\n");
        for (k, inner) in i.union.members.iter().enumerate() {
            let inner_symbol = inner.to_symbol();
            let inner_pascal = inner_symbol.pascal();
            let equals = if inner.name() == m.name() {
                String::from("arg")
            } else {
                i.construction(m)?
            };
            let else_str = if k == 0 { "" } else { "else " };
            s.push_str(
                format!(
                    "                {}if constexpr( std::is_same_v<T, {}> )\n",
                    else_str, inner_pascal
                )
                .as_str(),
            );
            s.push_str(format!("                    result = {};\n", equals).as_str());
        }
        s.push_str("                else\n");
        s.push_str(
            "                    static_assert(always_false_v<T>, \"non-exhaustive visitor!\");\n",
        );
        s.push_str("            }, myValue);\n");
        s.push_str("            return result;\n");
        s.push_str("        }");
        if j < i.union.members.len() - 1 {
            s.push_str("\n\n");
        }
    }
    Ok(s)
}

fn variants_get_is_decl(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("            bool getIs{}() const;", pascal).as_str());
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}
fn variants_get_is_def(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("        bool {}::getIs{}() const\n", clss, pascal).as_str());
        s.push_str("        {\n");
        s.push_str(format!("            return myValue.index() == {};\n", j).as_str());
        s.push_str("        }\n");
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}
fn variants_set_decl(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("            void set{}( {} value );", pascal, pascal).as_str());
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}
fn variants_set_def(i: &Info<'_>) -> Result<String> {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("        void {}::set{}( {} value )\n", clss, pascal, pascal).as_str());
        s.push_str("        {\n");
        s.push_str(format!("            myValue.emplace<{}>( value );\n", pascal).as_str());
        s.push_str("        }\n");
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    Ok(s)
}

fn variants_template_decl(i: &Info<'_>) -> String {
    let names: Vec<String> = i
        .union
        .members
        .iter()
        .map(|m| m.to_symbol())
        .map(|s| s.pascal().to_owned())
        .collect();
    names.join(", ")
}

fn variants_to_stream_decl(i: &Info<'_>) -> String {
    let mut s = String::new();
    let clss = i.union.name.pascal();
    for (j, m) in i.union.members.iter().enumerate() {
        let symbol = m.to_symbol();
        let pascal = symbol.pascal();
        s.push_str(format!("            if( value.getIs{}() )\n", pascal).as_str());
        s.push_str("            {\n");
        s.push_str(
            format!(
                "                toStream( os, value.getValue{}() );\n",
                pascal
            )
            .as_str(),
        );
        s.push_str("            }");
        if j < i.union.members.len() - 1 {
            s.push('\n');
        }
    }
    s
}
