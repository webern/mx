use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::attributes::{add_attributes_from_xml, Attributes};
use crate::xsd::constants::{ANNOTATION, ATTRIBUTE_GROUP, NAME, REF};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::{name_attribute, ref_attribute, Xsd};

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub enum AttributeGroup {
    Def(AttributeGroupDef),
    Ref(AttributeGroupRef),
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct AttributeGroupDef {
    pub id: Id,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub attributes: Attributes,
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct AttributeGroupRef {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub ref_: String,
}

impl AttributeGroup {
    pub fn id(&self) -> &Id {
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<AttributeGroup> {
        check!(ATTRIBUTE_GROUP, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        if let Some(_ref_) = node.attribute(REF) {
            Ok(AttributeGroup::Ref(AttributeGroupRef::from_xml(
                node, lineage, xsd,
            )?))
        } else {
            Ok(AttributeGroup::Def(AttributeGroupDef::from_xml(
                node, lineage, xsd,
            )?))
        }
    }

    pub fn is_ref(&self) -> bool {
        match self {
            AttributeGroup::Def(_) => false,
            AttributeGroup::Ref(_) => true,
        }
    }

    pub fn is_def(&self) -> bool {
        match self {
            AttributeGroup::Def(_) => true,
            AttributeGroup::Ref(_) => false,
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let name = name_attribute(node)?;
        let id = lineage.parent().unwrap();
        Ok(Self {
            id: id.clone(),
            name,
            annotation: Self::parse_annotation(node, lineage.clone(), xsd)?,
            attributes: add_attributes_from_xml(node, lineage.clone(), xsd)?,
        })
    }

    fn parse_annotation(
        node: &exile::Element,
        lineage: Lineage,
        xsd: &Xsd,
    ) -> Result<Option<Annotation>> {
        for inner in node.children() {
            if inner.name() == ANNOTATION {
                return Ok(Some(Annotation::from_xml(inner, lineage, xsd)?));
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let ref_ = ref_attribute(node)?;
        let id = lineage.parent().unwrap();
        Ok(Self {
            id: id.clone(),
            annotation: Self::parse_annotation(node, lineage.clone(), xsd)?,
            ref_,
        })
    }

    fn parse_annotation(
        node: &exile::Element,
        lineage: Lineage,
        xsd: &Xsd,
    ) -> Result<Option<Annotation>> {
        for inner in node.children() {
            if inner.name() == ANNOTATION {
                return Ok(Some(Annotation::from_xml(inner, lineage, xsd)?));
            }
        }
        Ok(None)
    }
}
