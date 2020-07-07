use crate::error::Result;
use crate::model::create::{Create, CreateError};
use crate::model::transform::Transform;
use crate::model::{DefaultCreate, Model};
use crate::xsd::{Entry, Xsd};
use std::borrow::Cow;
use std::fmt::{Debug, Formatter};

/// The 'Creator' takes a vector of `Create` objects, applies them to an `XSD` producing a model.
pub struct Creator {
    transforms: Vec<Box<dyn Transform>>,
    creates: Vec<Box<dyn Create>>,
}

impl Default for Creator {
    fn default() -> Self {
        Self {
            transforms: Vec::new(),
            creates: vec![Box::new(DefaultCreate::default())],
        }
    }
}

impl Creator {
    pub fn new(transforms: Vec<Box<dyn Transform>>, creates: Vec<Box<dyn Create>>) -> Self {
        Self {
            transforms,
            creates,
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
        for mut entry in xsd.entries() {
            let mut entry = entry.clone();
            for transform in &self.transforms {
                entry = wrap!(transform.transform(&entry, xsd))?;
            }
            let mut is_handled = false;
            for create in &self.creates {
                if let Some(mut more_models) = wrap!(create.create(&entry, xsd))? {
                    models.append(&mut more_models);
                    is_handled = true;
                    break;
                }
            }
            if !is_handled {
                return raise!(
                    "the entry {} was not handled by any Create objects",
                    entry.id()
                );
            }
            // TODO - if we get here then there was no handler, this is an error.
        }
        Ok(models)
    }

    fn transform<'a>(
        &'a self,
        entry: &'a Entry,
        xsd: &Xsd,
    ) -> std::result::Result<Cow<'a, Entry>, CreateError> {
        Ok(Cow::Borrowed(entry))
    }
}
