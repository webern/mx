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
use crate::xsd::choice::{Choice, ChoiceItem};
use crate::xsd::complex_type::{Children, ComplexType, Parent};
use crate::xsd::element::{ElementDef, ElementRef};
use crate::xsd::id::{Id, RootNodeType};
use crate::xsd::restriction::Facet;
use crate::xsd::{complex_type, element, simple_type, Entry, Xsd};
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
        let mut cloned = entry.clone();
        if let Entry::SimpleType(st) = &mut cloned {
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
                            // TODO - problem, this gets stripped off again by the tokenizer
                            enumer.push('_');
                        }
                    }
                }
            }
        }
        Ok(cloned)
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
        if let Id::Root(rid) = entry.id() {
            if rid.type_() == RootNodeType::ComplexType {
                if rid.name().as_ref() == "dynamics" {
                    return self.create_dynamics(entry);
                }
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

    fn create_dynamics(&self, entry: &Entry) -> CreateResult {
        let (ct, p, choice) = self.unwrap_dynamics(entry)?;
        let mut enumer = model::enumeration::Enumeration {
            /// TODO - get the name in a generic manner
            name: Symbol::new("dynamics enum"),
            members: vec![],
            documentation: entry.documentation(),
            other_field: None,
        };
        for c in &choice.choices {
            let mut is_other_field_found = false;
            if let Some(name) = self.unwrap_empty_element(c)? {
                enumer.members.push(Symbol::new(name));
            } else {
                let other_field_name = self.unwrap_other_field(c)?;
                if enumer.other_field.is_some() {
                    return Err(CreateError {
                        message: "create_dynamics: multiple 'other' fields found".to_string(),
                    });
                }
                enumer.other_field = Some(OtherField {
                    name: Symbol::new(other_field_name),
                    type_: BuiltinString::String,
                    wrapper_class_name: Symbol::new("dynamics value"),
                })
            }
        }
        // TODO - also create the 'wrapping' element named 'dynamics'
        Ok(Some(vec![Model::Enumeration(enumer)]))
    }

    fn unwrap_dynamics<'a>(
        &self,
        entry: &'a Entry,
    ) -> std::result::Result<(&'a ComplexType, &'a Parent, &'a Choice), CreateError> {
        let ct = if let Entry::ComplexType(ct) = entry {
            ct
        } else {
            return Err(CreateError {
                message: "create_dynamics: wrong type".to_string(),
            });
        };
        let p = if let complex_type::Payload::Parent(p) = &ct.payload {
            p
        } else {
            return Err(CreateError {
                message: "create_dynamics: expected parent".to_string(),
            });
        };
        let children = if let Some(children) = &p.children {
            children
        } else {
            return Err(CreateError {
                message: "create_dynamics: expected children".to_string(),
            });
        };
        let choice = if let Children::Choice(choice) = children {
            choice
        } else {
            return Err(CreateError {
                message: "create_dynamics: expected choice".to_string(),
            });
        };
        Ok((ct, p, choice))
    }

    fn unwrap_empty_element<'a>(
        &self,
        c: &'a ChoiceItem,
    ) -> std::result::Result<Option<&'a str>, CreateError> {
        let ref_: &ElementRef = if let ChoiceItem::Element(wrapped) = c {
            let ref_ = if let element::Element::Reference(ref_) = wrapped {
                ref_
            } else {
                return Err(CreateError {
                    message: "unwrap_empty_element: expected ElementDef".to_string(),
                });
            };
            ref_
        } else {
            return Err(CreateError {
                message: "unwrap_empty_element: expected Element".to_string(),
            });
        };

        if ref_.type_ == "empty" {
            Ok(Some(ref_.name.as_str()))
        } else {
            Ok(None)
        }
    }

    fn unwrap_simple_element<'a>(
        &self,
        c: &'a ChoiceItem,
    ) -> std::result::Result<Option<(&'a str, &'a str)>, CreateError> {
        let ref_: &ElementRef = if let ChoiceItem::Element(wrapped) = c {
            let ref_ = if let element::Element::Reference(ref_) = wrapped {
                ref_
            } else {
                return Err(CreateError {
                    message: "unwrap_empty_element: expected ElementDef".to_string(),
                });
            };
            ref_
        } else {
            return Err(CreateError {
                message: "unwrap_empty_element: expected Element".to_string(),
            });
        };

        /// TODO - checking of the type is janky
        if ref_.type_ == "xs:string" || ref_.type_ == "xs:token" {
            Ok(Some((ref_.name.as_str(), ref_.type_.as_str())))
        } else {
            Ok(None)
        }
    }

    fn unwrap_other_field<'a>(
        &self,
        c: &'a ChoiceItem,
    ) -> std::result::Result<&'a str, CreateError> {
        let found_stuff = if let Some(other_field) = self.unwrap_simple_element(c)? {
            other_field
        } else {
            return Err(CreateError {
                message: "create_dynamics: unable to unwrap the 'other' field".to_string(),
            });
        };
        if found_stuff.1 != "xs:string" {
            return return Err(CreateError {
                message: format!(
                    "unwrap_other_field: unsupported 'other' field type '{}'",
                    found_stuff.1
                ),
            });
        }
        Ok(found_stuff.0)
    }
}

fn create_pseudo_enum(entry: &Entry, spec: &PseudoEnumSpec) -> CreateResult {
    let mut enumer = model::enumeration::Enumeration {
        name: Symbol::new(spec.class_name.as_str()),
        members: vec![],
        documentation: entry.documentation(),
        other_field: Some(OtherField {
            name: Symbol::new(spec.extra_field_name.as_str()),
            // TODO - check if it's actually xs:token
            type_: BuiltinString::String,
            wrapper_class_name: Symbol::new(spec.class_name.as_str()),
        }),
    };
    for s in &spec.members {
        enumer.members.push(Symbol::new(s.as_str()));
    }
    Ok(Some(vec![Model::Enumeration(enumer)]))
}
