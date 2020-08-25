use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::common::DefinedBy;
use crate::xsd::constants::{ANNOTATION, ATTRIBUTE, DEFAULT, FIXED, NAME, REF, TYPE};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::{use_required, Xsd};

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Attribute {
    pub id: Id,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub defined_by: DefinedBy,
    pub required: bool,
    pub default: Option<String>,
    pub fixed: Option<String>,
}

impl Attribute {
    pub fn documentation(&self) -> String {
        if let Some(a) = &self.annotation {
            a.documentation()
        } else {
            "".to_owned()
        }
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(ATTRIBUTE, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let name = Self::parse_name(node);
        let defined_by = Self::parse_defined_by(node)?;
        Ok(Attribute {
            id,
            name,
            annotation: Self::parse_annotation(node, lineage, xsd)?,
            required: use_required(node),
            defined_by,
            default: node.attributes.map().get(DEFAULT).cloned(),
            fixed: node.attributes.map().get(FIXED).cloned(),
        })
    }

    fn parse_defined_by(node: &exile::Element) -> Result<DefinedBy> {
        if let Some(ref_) = node.attributes.map().get(REF) {
            Ok(DefinedBy::Ref(ref_.clone()))
        } else if let Some(type_) = node.attributes.map().get(TYPE) {
            Ok(DefinedBy::Type(type_.clone()))
        } else {
            raise!("could not find either '{}' or '{}' attribute", REF, TYPE)
        }
    }

    fn parse_name(node: &exile::Element) -> String {
        if let Some(name) = node.attributes.map().get(NAME) {
            name.clone()
        } else {
            "".to_owned()
        }
    }

    fn parse_annotation(
        node: &exile::Element,
        lineage: Lineage,
        xsd: &Xsd,
    ) -> Result<Option<Annotation>> {
        for child in node.children() {
            let t = child.name.as_str();
            match t {
                ANNOTATION => return Ok(Some(Annotation::from_xml(node, lineage, xsd)?)),
                _ => return raise!("unexpected node '{}'", t),
            }
        }
        Ok(None)
    }
}
