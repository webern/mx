use crate::error::Result;
use crate::xsd::annotation::Annotation;

use crate::xsd::attributes::{add_attributes_from_xml, Attributes};
use crate::xsd::constants::{ANNOTATION, REF};
use crate::xsd::{name_attribute, ref_attribute, EntryType, ID};

pub enum AttributeGroup {
    Def(AttributeGroupDef),
    Ref(AttributeGroupRef),
}

pub struct AttributeGroupDef {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub attributes: Attributes,
}

pub struct AttributeGroupRef {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub ref_: String,
}

impl AttributeGroup {
    pub fn id(&self) -> &ID {
        match self {
            AttributeGroup::Def(x) => &x.id,
            AttributeGroup::Ref(x) => &x.id,
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
        if let Some(_ref_) = node.attributes.map().get(REF) {
            Ok(AttributeGroup::Ref(AttributeGroupRef::from_xml(
                node, index,
            )?))
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

impl AttributeGroupRef {
    pub fn documentation(&self) -> String {
        if let Some(a) = &self.annotation {
            a.documentation()
        } else {
            "".to_owned()
        }
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let ref_ = ref_attribute(node)?;
        let id = ID {
            entry_type: EntryType::Other("attributeGroup:ref".into()),
            name: ref_.clone(),
        };
        Ok(Self {
            id,
            index,
            annotation: Self::parse_annotation(node, index)?,
            ref_,
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
