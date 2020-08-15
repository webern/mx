use crate::error::Result;
use crate::model::create::{Create, CreateError};
use crate::model::post_process::PostProcess;
use crate::model::transform::Transform;
use crate::model::{DefaultCreate, Def, Model};
use crate::xsd::{Entry, Xsd};
use std::borrow::Cow;
use std::fmt::{Debug, Formatter};

/// The 'Creator' takes a vector of `Create` objects, applies them to an `XSD` producing a model.
pub struct Creator {
    transforms: Option<Vec<Box<dyn Transform>>>,
    creates: Vec<Box<dyn Create>>,
    post_processors: Option<Vec<Box<dyn PostProcess>>>,
}

impl Default for Creator {
    fn default() -> Self {
        Self {
            transforms: None,
            creates: vec![Box::new(DefaultCreate::default())],
            post_processors: None,
        }
    }
}

impl Creator {
    /// Create a new `Creator` object by passing in the transform and create objects.
    pub fn new(
        transforms: Option<Vec<Box<dyn Transform>>>,
        creates: Vec<Box<dyn Create>>,
        post_processors: Option<Vec<Box<dyn PostProcess>>>,
    ) -> Self {
        Self {
            transforms,
            creates,
            post_processors,
        }
    }

    /// Create a new `Creator` object by passing in the tranform and create objects. This function
    /// will append the default `Create` implementation as the last last `Create` object.
    pub fn new_with_default(
        transforms: Option<Vec<Box<dyn Transform>>>,
        mut creates: Option<Vec<Box<dyn Create>>>,
        post_processors: Option<Vec<Box<dyn PostProcess>>>,
    ) -> Self {
        let mut creates = if let Some(the_goods) = creates {
            the_goods
        } else {
            Vec::new()
        };
        creates.push(Box::new(DefaultCreate::default()));
        Self {
            transforms,
            creates,
            post_processors,
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
    pub fn create(&self, xsd: &Xsd) -> Result<Model> {
        let mut defs: Vec<Def> = Vec::new();
        for mut entry in xsd.entries() {
            let mut entry = entry.clone();
            if let Some(transforms) = &self.transforms {
                for transform in transforms {
                    entry = wrap!(transform.transform(&entry, xsd))?;
                }
            }
            let mut is_handled = false;
            for create in &self.creates {
                if let Some(mut more_models) = wrap!(create.create(&entry, xsd))? {
                    is_handled = true;
                    if let Some(post_processors) = &self.post_processors {
                        let mut post_processed_models = Vec::new();
                        for model in &more_models {
                            let mut model_being_processed = model.clone();
                            for post_processor in post_processors {
                                model_being_processed =
                                    wrap!(post_processor.process(&model_being_processed, xsd))?;
                            }
                            post_processed_models.push(model_being_processed);
                        }
                        more_models = post_processed_models;
                    }
                    defs.append(&mut more_models);
                    break;
                }
            }
            if !is_handled {
                return raise!(
                    "the entry {} was not handled by any Create objects",
                    entry.id()
                );
            }
        }
        let mut model = Model::default();
        for def in defs {
            model.add(def);
        }
        Ok(model)
    }

    fn transform<'a>(
        &'a self,
        entry: &'a Entry,
        xsd: &Xsd,
    ) -> std::result::Result<Cow<'a, Entry>, CreateError> {
        Ok(Cow::Borrowed(entry))
    }
}
