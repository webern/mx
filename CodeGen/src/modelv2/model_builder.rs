use super::Model;
use crate::error::Result;
use crate::xsd;
use crate::xsd::Entry;

#[derive(Clone, Debug, Default)]
pub struct ModelBuilder {
    fake: String,
}

impl ModelBuilder {
    /// Instantiate a new `ModelBuilder`. No different than `Default`.
    pub fn new() -> Self {
        Self::default()
    }

    pub fn add(&mut self, entry: Entry) -> Result<()> {
        raise!("not implemented")
    }

    pub fn add_all(&mut self, entry: Entry) -> Result<()> {
        raise!("not implemented")
    }

    /// Build the model. Can error if the internal Xsd is not comprehensible.
    pub fn build(&mut self) -> Result<Model> {
        raise!("not implemented")
    }
}
