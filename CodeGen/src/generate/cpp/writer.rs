use crate::error::Result;
use crate::generate::paths::Paths;
use crate::model::Model;

#[derive(Debug, Clone)]
pub struct Writer {
    pub models: Vec<Model>,
    pub paths: Paths,
}

impl Writer {
    pub fn new(models: Vec<Model>) -> Self {
        Self {
            models,
            paths: Paths::default(),
        }
    }

    pub fn write_code(&self) -> Result<()> {
        self.write_enums()?;
        Ok(())
    }
}
