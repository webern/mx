use crate::model::symbol::Symbol;
use crate::xsd::element::Element as XsdElement;

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Element {
    name: Symbol,
    inner: XsdElement,
}

impl Element {
    pub fn new(xsd: XsdElement) -> Self {
        Self {
            name: Symbol::new(xsd.name()),
            inner: xsd,
        }
    }
    pub fn name(&self) -> &Symbol {
        &self.name
    }
}
