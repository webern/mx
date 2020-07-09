use crate::generate::cpp::constants::{
    enum_member_substitutions, pseudo_enums, reserved_words, suffixed_enum_names, PseudoEnumSpec,
};
use crate::model;
use crate::model::builtin::BuiltinString;
use crate::model::create::{Create, CreateError, CreateResult};
use crate::model::enumeration::OtherField;
use crate::model::symbol::Symbol;
use crate::model::transform::Transform;
use crate::model::Model::Enumeration;
use crate::model::{DefaultCreate, Model};
use crate::xsd::restriction::Facet;
use crate::xsd::{simple_type, Entry, Xsd};
use indexmap::set::IndexSet;
use std::collections::HashMap;

#[derive(Debug, Clone)]
pub struct MxModeler {
    enum_member_substitutions: HashMap<String, String>,
    suffixed_enum_names: IndexSet<String>,
    reserved_words: IndexSet<String>,
    pseudo_enums: HashMap<String, PseudoEnumSpec>,
}

impl Transform for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn transform(&self, entry: &Entry, xsd: &Xsd) -> Result<Entry, CreateError> {
        let mut entry = entry.clone();
        if let Entry::SimpleType(st) = &mut entry {
            let symbolized = Symbol::new(st.name.as_str());
            if self.suffixed_enum_names.contains(symbolized.pascal()) {
                st.name.push_str("-enum");
            }
            if let simple_type::Payload::Restriction(res) = &mut st.payload {
                for facet in &mut res.facets {
                    if let Facet::Enumeration(enumer) = facet {
                        // do explicit enumeration member renames, e.g. "16th" to "sixteenth"
                        if let Some(to) = self.enum_member_substitutions.get(enumer) {
                            enumer.clear();
                            enumer.push_str(to);
                        }
                        // do reserved word renames, e.g. "continue" to "continue_"
                        if self.reserved_words.contains(enumer.as_str()) {
                            enumer.push('_');
                        }
                    }
                }
            }
        }
        Ok(entry)
    }
}

impl Create for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn create(&self, entry: &Entry, xsd: &Xsd) -> CreateResult {
        // TODO - boo - change the keys to match exactly once old impl is gone
        for (k, v) in &self.pseudo_enums {
            if k.contains(entry.id().display().as_str()) {
                return create_pseudo_enum(entry, v);
            }
        }
        Ok(None)
    }
}

impl MxModeler {
    pub fn new() -> Self {
        Self {
            enum_member_substitutions: enum_member_substitutions(),
            suffixed_enum_names: suffixed_enum_names(),
            reserved_words: reserved_words(),
            pseudo_enums: pseudo_enums(),
        }
    }
}

fn create_pseudo_enum(entry: &Entry, spec: &PseudoEnumSpec) -> CreateResult {
    let mut enumer = model::enumeration::Enumeration {
        name: Symbol::new(spec.class_name.as_str()),
        members: vec![],
        documentation: entry.documentation(),
        other_field: Some(OtherField {
            name: Symbol::new(spec.extra_field_name.as_str()),
            // TODO - check in case it's actually xs:token
            type_: BuiltinString::String,
        }),
    };
    for s in &spec.members {
        enumer.members.push(Symbol::new(s.as_str()));
    }
    Ok(Some(vec![Model::Enumeration(enumer)]))
}
