use crate::model::create::{CreateError, CreateResult};
use crate::model::enumeration::Enumeration;
use crate::model::symbol::Symbol;
use crate::model::Def;
use crate::xsd::restriction::Facet;
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::{simple_type, Xsd};

pub(super) fn is_enumeration(st: &SimpleType) -> bool {
    match &st.payload {
        Payload::Restriction(r) => {
            if r.facets.is_empty() {
                return false;
            }
            for facet in &r.facets {
                match facet {
                    Facet::Enumeration(_) => continue,
                    _ => return false,
                }
            }
            return true;
        }
        _ => false,
    }
}

pub(super) fn model_enumeration(st: &SimpleType, xsd: &Xsd) -> CreateResult {
    let restriction = if let simple_type::Payload::Restriction(r) = &st.payload {
        r
    } else {
        return Err(CreateError::new("expected restriction"));
    };
    let mut members = Vec::new();
    for facet in &restriction.facets {
        let s = if let Facet::Enumeration(s) = facet {
            s
        } else {
            return Err(CreateError::new("expected enumeration"));
        };
        members.push(Symbol::new(s.as_str()));
    }
    let enm = Enumeration {
        name: Symbol::new(st.name.as_str()),
        members,
        documentation: st.documentation(),
        other_field: None,
    };
    Ok(Some(vec![Def::Enumeration(enm)]))
}
