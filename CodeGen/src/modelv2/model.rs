use crate::model::symbol::Symbol;

#[derive(Clone, Debug, Default)]
pub struct Model {
    fake: Symbol,
}

impl Model {
    fn name(&self, id: &str) -> &Symbol {
        &self.fake
    }
}

pub struct Element<'a> {
    id: String,
    model: &'a Model,
}

impl Element<'_> {
    pub fn name(&self, id: &str) -> &Symbol {
        self.model.name(id)
    }
}
