use crate::model::create::{Create, CreateError, CreateResult};
use crate::model::transform::Transform;
use crate::xsd::{Entry, Xsd};
use std::borrow::Cow;

#[derive(Debug, Clone, Copy)]
pub struct MxModeler {}

impl Transform for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn transform(&self, entry: &Entry, xsd: &Xsd) -> Result<Entry, CreateError> {
        Ok(entry.clone())
    }
}

impl Create for MxModeler {
    fn name(&self) -> &'static str {
        "mx-cpp"
    }

    fn create(&self, entry: &Entry, xsd: &Xsd) -> CreateResult {
        Ok(None)
    }
}

impl MxModeler {
    pub fn new() -> Self {
        Self {}
    }
}
