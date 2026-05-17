use crate::model::create::CreateError;
use crate::model::Def;
use crate::xsd::Xsd;

/// Changes a `Model`
pub trait PostProcess {
    /// The name of this `PostProcess` object (for debugging).
    fn name(&self) -> &'static str;

    /// Transforms and `Entry`.
    fn process(&self, model: &Def, xsd: &Xsd) -> std::result::Result<Def, CreateError>;
}
