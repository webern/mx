use crate::model::create::CreateError;
use crate::model::Model;
use crate::xsd::{Entry, Xsd};
use std::fmt::{Display, Formatter};

/// # Create Trait
///
/// An object that takes a top-level `Entry` from the XSD document, alters it in some way, and
/// returns the altered `Entry`.
///
pub trait Transform {
    /// The name of this `Transform` object (for debugging).
    fn name(&self) -> &'static str;

    /// Transforms and `Entry`.
    fn transform(&self, entry: &Entry, xsd: &Xsd) -> std::result::Result<Entry, CreateError>;
}
