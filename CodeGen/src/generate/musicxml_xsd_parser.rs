use crate::generate::musicxml_xsd::MusicXSD;

use crate::generate::musicxml_xsd_parser::Error::SchemaNotFound;
use derive_more::Display;
use exile::{Document, Element};
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::sync::atomic::{AtomicUsize, Ordering};

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub(crate) enum Error {
    SchemaNotFound,
    UnknownSchemaNode(String),
    NameAttributeNotFound(String),
    XlinkNamespaceNotFound,
    DuplicateXsInfoID(String),
}

static COUNTER: AtomicUsize = AtomicUsize::new(0);

pub(crate) fn parse_musicxml_xsd(doc: &Document) -> Result<MusicXSD, Error> {
    let root = doc.root();
    if root.fullname().as_str() != "xs:schema" {
        return Err(SchemaNotFound);
    }
    let mut type_nodes = parse_type_nodes(doc)?;
    let something = dobeedobeedo(&type_nodes)?;
    for (_, n) in type_nodes {
        println!("{}", n);
    }

    Ok(MusicXSD {})
}

fn parse_type_nodes(doc: &Document) -> Result<HashMap<String, XsdTypeNode>, Error> {
    let mut type_nodes = HashMap::new();
    for node in doc.root().children() {
        let x = parse_xs_info(node)?;
        if let Some(existing) = type_nodes.insert(x.id.clone(), x) {
            return Err(Error::DuplicateXsInfoID(existing.id.clone()));
        }
    }
    Ok(type_nodes)
}

const XS_ANNOTATION: &str = "xs:annotation";
const XS_ATTRIBUTE_GROUP: &str = "xs:attributeGroup";
const XS_COMPLEX_TYPE: &str = "xs:complexType";
const XS_ELEMENT: &str = "xs:element";
const XS_GROUP: &str = "xs:group";
const XS_IMPORT: &str = "xs:import";
const XS_SIMPLE_TYPE: &str = "xs:simpleType";

#[derive(Debug, Clone)]
pub(crate) struct XsdTypeNode<'a> {
    pub(crate) xsd_type: XsdType,
    pub(crate) index: usize,
    pub(crate) id: String,
    pub(crate) name: String,
    pub(crate) node: &'a Element,
}

impl<'a> Display for XsdTypeNode<'_> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}-{}-{}", self.index, self.id, self.name)
    }
}

fn parse_xs_info(node: &Element) -> Result<XsdTypeNode, Error> {
    let index = COUNTER.fetch_add(1, Ordering::Relaxed);
    let xsd_type = XsdType::parse(node)?;
    let name = match xsd_type {
        XsdType::XsAnnotation => format!("{}:{}", XS_ANNOTATION, index),
        XsdType::XsImport => format!(
            "{}:{}",
            XS_IMPORT,
            node.attributes
                .map()
                .get("namespace")
                .ok_or_else(|| Error::NameAttributeNotFound(node.fullname()))?
        ),
        _ => node
            .attributes
            .map()
            .get("name")
            .ok_or_else(|| Error::XlinkNamespaceNotFound)?
            .clone(),
    };
    let id = match xsd_type {
        XsdType::XsAnnotation | XsdType::XsImport => name.clone(),
        _ => format!("{}:{}", node.fullname(), name),
    };
    Ok(XsdTypeNode {
        xsd_type,
        index,
        id,
        name,
        node,
    })
}

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub(crate) enum XsdType {
    XsAnnotation,
    XsAttributeGroup,
    XsComplexType,
    XsElement,
    XsGroup,
    XsImport,
    XsSimpleType,
}

impl XsdType {
    fn parse(node: &Element) -> Result<Self, Error> {
        let fullname = node.fullname();
        match fullname.as_str() {
            XS_ANNOTATION => Ok(XsdType::XsAnnotation),
            XS_ATTRIBUTE_GROUP => Ok(XsdType::XsAttributeGroup),
            XS_COMPLEX_TYPE => Ok(XsdType::XsComplexType),
            XS_ELEMENT => Ok(XsdType::XsElement),
            XS_GROUP => Ok(XsdType::XsGroup),
            XS_IMPORT => Ok(XsdType::XsImport),
            XS_SIMPLE_TYPE => Ok(XsdType::XsSimpleType),
            _ => Err(Error::UnknownSchemaNode(fullname)),
        }
    }
}

enum TypeDefinition {
    Simple(SimpleType),
}

enum SimpleType {
    Enum(Enum),
}

struct Enum {}

fn dobeedobeedo(xsd_nodes: &HashMap<String, XsdTypeNode>) -> Result<Vec<TypeDefinition>, Error> {
    for (_, x) in xsd_nodes {
        if (x.xsd_type != XsdType::XsSimpleType) {
            continue;
        }
        if !is_enumeration_simple_type(x.node) {
            continue;
        }
        println!("enumeration");
    }
    Ok(Vec::new())
}

fn is_enumeration_simple_type(node: &Element) -> bool {
    for child in node.children() {
        if child.fullname() == "xs:restriction" {
            for restriction in child.children() {
                if restriction.fullname() == "xs:enumeration" {
                    return true;
                }
            }
        }
    }
    false
}
