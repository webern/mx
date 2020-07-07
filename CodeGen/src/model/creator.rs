use crate::error::Result;
use crate::model::create::Create;
use crate::model::{DefaultCreate, Model};
use crate::xsd::Xsd;
use std::fmt::{Debug, Formatter};

/// The 'Creator' takes a vector of `Create` objects, applies them to an `XSD` producing a model.
pub struct Creator {
    creates: Vec<Box<dyn Create>>,
}

impl Default for Creator {
    fn default() -> Self {
        Self {
            creates: vec![Box::new(DefaultCreate::default())],
        }
    }
}

impl Debug for Creator {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Creator{{ creates: [")?;
        for create in &self.creates {
            write!(f, "{},", create.name())?;
        }
        write!(f, "] }}")
    }
}

impl Creator {
    pub fn create(&self, xsd: &Xsd) -> Result<Vec<Model>> {
        let mut models = Vec::new();
        for entry in xsd.entries() {
            for create in &self.creates {
                if let Some(mut more_models) = wrap!(create.create(entry, xsd))? {
                    models.append(&mut more_models);
                    break;
                }
            }
            // TODO - if we get here then there was no handler, this is an error.
        }
        Ok(models)
    }
}
