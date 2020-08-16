use crate::model::enumeration::Enumeration;
use crate::error::Result;
use crate::model::Def;

pub(super) fn enum_default(e: &Enumeration) -> String {
    format!("{}::{}", e.name.pascal(), e.default.camel())
}

pub(super) fn default_construct(d: &Def) -> String {
    match d {
        Def::Enumeration(e) => enum_default(e),
        _ => format!("{}::{{}}", d.name().pascal())
    }
}