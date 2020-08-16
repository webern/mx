use crate::generate::cpp::constants::{
    custom_scalar_strings, enum_member_substitutions, pseudo_enums, reserved_words,
    suffixed_enum_names, suffixed_value_names, PseudoEnumSpec,
};
use crate::model;
use crate::model::builtin::BuiltinString;
use crate::model::create::{Create, CreateError, CreateResult};
use crate::model::enumeration::OtherField;
use crate::model::post_process::PostProcess;
use crate::model::scalar::{Bound, NumericData, Range, ScalarNumeric};
use crate::model::symbol::Symbol;
use crate::model::transform::Transform;
use crate::model::Def::Enumeration;
use crate::model::{DefaultCreate, Def};
use crate::xsd::choice::{Choice, ChoiceItem};
use crate::xsd::complex_type::{Children, ComplexType, Parent};
use crate::xsd::element::{ElementDef, ElementRef};
use crate::xsd::id::{Id, RootNodeType};
use crate::xsd::primitives::Numeric;
use crate::xsd::primitives::{BaseType, Character, PrefixedString, Primitive};
use crate::xsd::restriction::Facet;
use crate::xsd::{complex_type, element, simple_type, Entry, Xsd};
use indexmap::set::IndexSet;
use std::collections::HashMap;

#[derive(Debug, Clone)]
pub struct MxModeler {
    enum_member_substitutions: HashMap<String, String>,
    suffixed_enum_names: IndexSet<String>,
    suffixed_value_names: HashMap<String, String>,
    reserved_words: IndexSet<String>,
    pseudo_enums: HashMap<String, PseudoEnumSpec>,
    custom_scalar_strings: IndexSet<&'static str>,
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
        }
        Ok(cloned)
    }
}

impl Create for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn create(&self, entry: &Entry, xsd: &Xsd) -> CreateResult {
        if let Some(spec) = self.pseudo_enums.get(entry.id().display().as_str()) {
            return create_pseudo_enum(entry, spec);
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

impl PostProcess for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn process(&self, model: &Def, xsd: &Xsd) -> Result<Def, CreateError> {
        if let Def::Enumeration(enumer) = model {
            let mut cloned = enumer.clone();
            for (i, member) in cloned.members.iter_mut().enumerate() {
                // add an underscore as a suffix to camel case representations that would otherwise
                // collide with reserved words in C++.
                if self.reserved_words.contains(member.camel()) {
                    let mut replacement = member.camel().to_owned();
                    replacement.push('_');
                    member.set_camel(replacement)
                }
                // replace certain enum representations that would be illegal in C++, e.g. 16th.
                if let Some(replacement) = self.enum_member_substitutions.get(member.original()) {
                    member.replace(replacement);
                }
                // replace an empty string value with some kind of symbol name.
                if member.original() == "" {
                    member.replace("emptystring");
                }
                if i == 0 && member.original() == cloned.default.original() {
                    if member.renamed_to() != cloned.default.renamed_to() {
                        cloned.default.replace(member.renamed_to());
                    }
                    if member.camel() != cloned.default.camel() {
                        cloned.default.set_camel(member.camel());
                    }
                }
            }
            return Ok(Def::Enumeration(cloned));
        } else if let Def::ScalarString(scalar_string) = model {
            if self
                .custom_scalar_strings
                .contains(scalar_string.name.original())
            {
                return Ok(Def::CustomScalarString(scalar_string.clone()));
            } else if let Some(new_name) =
                self.suffixed_value_names.get(scalar_string.name.original())
            {
                let mut st = scalar_string.clone();
                st.name.replace(&new_name);
                return Ok(Def::ScalarString(st));
            }
        } else if let Def::ScalarNumber(scalar_number) = model {
            match scalar_number {
                ScalarNumeric::Decimal(n) => {
                    if let Some(new_name) = self.suffixed_value_names.get(n.name.original()) {
                        let mut n = n.clone();
                        n.name.replace(&new_name);
                        return Ok(Def::ScalarNumber(ScalarNumeric::Decimal(n)));
                    }
                }
                ScalarNumeric::Integer(n) => {
                    let mut is_changed = false;
                    let n = if n.name.original() == "accordion-middle" {
                        let mut changed = n.clone();
                        changed.range.min = Some(Bound::Inclusive(0));
                        changed.documentation.push_str(
                            "\n\nNote: MusicXML specifies the minimum allowable value \
                        as 1, however test documents exist that have a value of 0. This library supports \
                        a minimum value of 0. Per https://github.com/w3c/musicxml/issues/134, the correct \
                        representation for 0 dots is to omit the element, so it is possible to create \
                        invalid MusicXML by setting the value to 0 here.",
                        );
                        is_changed = true;
                        changed
                    } else {
                        n.clone()
                    };
                    // note: accordion-middle is one of the values that is renamed here. if it were
                    // not, it would need to be explicitly returned.
                    if let Some(new_name) = self.suffixed_value_names.get(n.name.original()) {
                        is_changed = true;
                        let mut mut_num = n.clone();
                        mut_num.name.replace(&new_name);
                        return Ok(Def::ScalarNumber(ScalarNumeric::Integer(mut_num)));
                    }
                }
            }
        } else if let Def::DerivedSimpleType(derived) = model {
            match derived.name.original() {
                "positive-divisions" => {
                    let replacement = NumericData {
                        name: derived.name.clone(),
                        base_type: Numeric::Decimal,
                        documentation: derived.documentation.clone(),
                        range: Range {
                            min: Some(Bound::Exclusive(0.0)),
                            max: None,
                        },
                    };
                    return Ok(Def::ScalarNumber(ScalarNumeric::Decimal(replacement)));
                }
                unhandled => {
                    return Err(CreateError {
                        message: format!("Unhandled DerivedSimpleType: '{}'", unhandled),
                    })
                }
            }
        }
        Ok(model.clone())
    }
}

impl MxModeler {
    pub fn new() -> Self {
        Self {
            enum_member_substitutions: enum_member_substitutions(),
            suffixed_enum_names: suffixed_enum_names(),
            suffixed_value_names: suffixed_value_names(),
            reserved_words: reserved_words(),
            pseudo_enums: pseudo_enums(),
            custom_scalar_strings: custom_scalar_strings(),
        }
    }

    fn create_dynamics(&self, entry: &Entry) -> CreateResult {
        let (ct, p, choice) = self.unwrap_dynamics(entry)?;
        let mut enumer = model::enumeration::Enumeration {
            /// TODO - get the name in a generic manner
            name: Symbol::new("dynamics enum"),
            members: vec![],
            documentation: entry.documentation(),
            default: Symbol::new("mf"),
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
        Ok(Some(vec![Def::Enumeration(enumer)]))
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

        if ref_.type_ == BaseType::Custom("empty".to_owned()) {
            Ok(Some(ref_.name.as_str()))
        } else {
            Ok(None)
        }
    }

    fn unwrap_simple_element<'a>(
        &self,
        c: &'a ChoiceItem,
    ) -> std::result::Result<Option<(&'a str, &'a BaseType)>, CreateError> {
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

        /// TODO - allow all base types?
        if ref_.type_.is_string() || ref_.type_.is_token() {
            Ok(Some((ref_.name.as_str(), &ref_.type_)))
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
        if *found_stuff.1 != BaseType::String {
            return return Err(CreateError {
                message: format!(
                    "unwrap_other_field: unsupported 'other' field type '{}'",
                    found_stuff.1.name()
                ),
            });
        }
        Ok(found_stuff.0)
    }
}

fn create_pseudo_enum(entry: &Entry, spec: &PseudoEnumSpec) -> CreateResult {
    let original = entry.id().name();
    let rename = format!("{}-enum", original);
    let mut symbol = Symbol::new(original);
    symbol.replace(rename);
    let mut enumer = model::enumeration::Enumeration {
        name: symbol,
        members: vec![],
        documentation: entry.documentation(),
        default: Symbol::new(spec.default_value.as_str()),
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
    Ok(Some(vec![Def::Enumeration(enumer)]))
}
