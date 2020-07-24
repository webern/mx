#[macro_use]
use crate::model::create::{CreateError, CreateResult};
use crate::model::scalar_string::ScalarString;
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::xsd::primitives::{BaseType, Character, Primitive};
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::Xsd;

pub(super) fn model_scalar_string(st: &SimpleType, _xsd: &Xsd) -> Option<CreateResult> {
    match &st.payload {
        Payload::Restriction(r) => {
            if r.facets.is_empty() {
                return None;
            }
            let base_type = if let BaseType::Primitive(p) = r.base {
                p
            } else {
                // not a primitive type (such as string)
                return None;
            };
            let base_type = if let Primitive::Character(c) = base_type {
                c
            } else {
                // not a character (i.e. string) type
                return None;
            };
            let mut scalar_string = match base_type {
                Character::Language | Character::Name | Character::NormalizedString => {
                    return some_create_err!("'{}' is not supported", base_type);
                }
                _ => ScalarString {
                    name: Symbol::new(st.name.as_str()),
                    base_type,
                    documentation: st.documentation(),
                    ..Default::default()
                },
            };
            for facet in &r.facets {
                match facet {
                    Facet::Enumeration(_)
                    | Facet::MaxExclusive(_)
                    | Facet::MaxInclusive(_)
                    | Facet::MinExclusive(_)
                    | Facet::MinInclusive(_) => {
                        return some_create_err!("unsupported facet '{:?}'", facet)
                    }
                    Facet::Length(l) => {
                        scalar_string.min_length = Some(*l);
                        scalar_string.max_length = Some(*l);
                    }
                    Facet::MaxLength(l) => scalar_string.max_length = Some(*l),
                    Facet::MinLength(l) => scalar_string.min_length = Some(*l),
                    Facet::Pattern(p) => scalar_string.pattern = Some(p.clone()),
                }
            }
            return Some(Ok(Some(vec![Model::ScalarString(scalar_string)])));
        }
        _ => return None,
    }
}
