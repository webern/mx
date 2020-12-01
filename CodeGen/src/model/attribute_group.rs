use crate::model::symbol::Symbol;

#[derive(Debug, Clone, PartialEq, PartialOrd)]
pub struct AttributeGroup {
    name: Symbol,
    inner: crate::xsd::attribute_group::AttributeGroup,
}

impl AttributeGroup {
    pub fn new(xsd: crate::xsd::attribute_group::AttributeGroup) -> Self {
        Self {
            name: Symbol::new(match &xsd {
                crate::xsd::attribute_group::AttributeGroup::Def(d) => d.name.as_str(),
                crate::xsd::attribute_group::AttributeGroup::Ref(r) => r.ref_.as_str(),
            }),
            inner: xsd,
        }
    }
    pub fn name(&self) -> &Symbol {
        &self.name
    }
}
