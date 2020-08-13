use crate::error::Result;
use crate::generate::cpp::cpp_template::{render_core_cpp, render_core_h};
use crate::generate::cpp::writer::Writer;
use crate::generate::template::{
    render, CORE_H, DECIMAL_BUILTINS_CPP, DECIMAL_BUILTINS_H, DECIMAL_TYPE_CPP, DECIMAL_TYPE_H,
    INTEGER_BUILTINS_CPP, INTEGER_BUILTINS_H, INTEGER_TYPE_CPP, INTEGER_TYPE_H, NO_DATA,
};
use crate::model::scalar::{Bound, NumericData, Range, UnionData};
use crate::model::scalar::{ScalarNumeric, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::utils::string_stuff::{documentation, sep, write_documentation};
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::Primitive;
use crate::xsd::primitives::{BaseType, PrefixedString};
use indexmap::Equivalent;
use std::collections::HashMap;
use std::fs::OpenOptions;
use std::io::Write;
use std::ops::Deref;

impl Writer {
    pub(crate) fn write_unions(&self, mut unions: &[&UnionData]) -> Result<()> {
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
