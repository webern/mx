use crate::model::create::CreateError;
use crate::model::Model;
use crate::xsd::Xsd;

/// Changes a `Model`
pub trait PostProcess {
    /// The name of this `PostProcess` object (for debugging).
    fn name(&self) -> &'static str;

    /// Transforms and `Entry`.
    fn process(&self, model: &Model, xsd: &Xsd) -> std::result::Result<Model, CreateError>;
}
