use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::attribute::Attribute;
use crate::xsd::attributes::{add_attributes_from_xml, Attributes};
use crate::xsd::constants::{ANNOTATION, REF};
use crate::xsd::{name_attribute, EntryType, ID};

pub enum AttributeGroup {
    Def(AttributeGroupDef),
    Ref(String),
}

pub struct AttributeGroupDef {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub attributes: Attributes,
}

impl AttributeGroup {
    pub fn id(&self) -> &ID {
        match self {
            AttributeGroup::Def(x) => &x.id,
            AttributeGroup::Ref(x) => {
                static BOGUS: ID = ID {
                    entry_type: EntryType::Other("AttributeGroup::Ref".to_owned()),
                    name: "".to_string(),
                };
                // TODO - this is dumb
                &BOGUS
            }
        }
    }

    pub fn documentation(&self) -> String {
        if let AttributeGroup::Def(def) = self {
            def.documentation()
        } else {
            "".to_owned()
        }
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<AttributeGroup> {
        if let Some(ref_) = node.attributes.map().get(REF) {
            Ok(AttributeGroup::Ref(ref_.clone()))
        } else {
            Ok(AttributeGroup::Def(AttributeGroupDef::from_xml(
                node, index,
            )?))
        }
    }
}

impl AttributeGroupDef {
    pub fn documentation(&self) -> String {
        if let Some(a) = &self.annotation {
            a.documentation()
        } else {
            "".to_owned()
        }
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let name = name_attribute(node)?;
        let id = ID {
            entry_type: EntryType::AttributeGroup,
            name: name.clone(),
        };
        Ok(Self {
            id,
            index,
            name,
            annotation: Self::parse_annotation(node, index)?,
            attributes: add_attributes_from_xml(node, index)?,
        })
    }

    fn parse_annotation(node: &exile::Element, index: u64) -> Result<Option<Annotation>> {
        for inner in node.children() {
            if inner.name.as_str() == ANNOTATION {
                return Ok(Some(Annotation::from_xml(inner, index)?));
            }
        }
        Ok(None)
    }
}
