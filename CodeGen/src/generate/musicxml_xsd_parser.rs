use super::musicxml_xsd::{Enumeration, MusicXSD, SimpleType, TypeDefinition};

use crate::generate::cpp::constants::PseudoEnumSpec;
use crate::generate::musicxml_xsd_parser::Error::SchemaNotFound;
use crate::generate::musicxml_xsd_parser::XsType::ComplexType;
use crate::generate::mx_enum_writer::MxEnumOption;
use crate::utils::string_stuff::{camel_case, pascal_case, Symbol};
use derive_more::Display;
use exile::{Document, Element};
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::sync::atomic::{AtomicUsize, Ordering};

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Error {
    SchemaNotFound,
    UnknownSchemaNode(String),
    NameAttributeNotFound(String),
    XlinkNamespaceNotFound,
    DuplicateXsInfoID(String),
    UnexpectedNode(String),
    SimpleTypeBaseNotFound,
    ExpectedOneChildOfXsAnnotation,
    ExpectedDocumentationNode,
    ExpectedDocumentationNodeToReturnText,
}

static COUNTER: AtomicUsize = AtomicUsize::new(0);

pub struct XsdParserParams {
    pub pseudo_enums: HashMap<String, PseudoEnumSpec>,
}

pub fn parse_musicxml_xsd(doc: &Document, params: XsdParserParams) -> Result<MusicXSD, Error> {
    let root = doc.root();
    if root.fullname().as_str() != "xs:schema" {
        return Err(SchemaNotFound);
    }
    let type_nodes = parse_type_nodes(doc)?;
    let type_definitions = parse_type_definitions(&type_nodes, &params)?;
    Ok(MusicXSD::new(type_definitions))
}

fn parse_type_nodes(doc: &Document) -> Result<HashMap<String, XsTypeNode<'_>>, Error> {
    let mut type_nodes = HashMap::new();
    for node in doc.root().children() {
        let x = parse_xs_info(node)?;
        if let Some(existing) = type_nodes.insert(x.id.clone(), x) {
            return Err(Error::DuplicateXsInfoID(existing.id));
        }
    }
    Ok(type_nodes)
}

const XS_ANNOTATION: &str = "xs:annotation";
const XS_ATTRIBUTE_GROUP: &str = "xs:attributeGroup";
const XS_CHOICE: &str = "xs:choice";
const XS_COMPLEX_TYPE: &str = "xs:complexType";
const XS_ELEMENT: &str = "xs:element";
const XS_GROUP: &str = "xs:group";
const XS_IMPORT: &str = "xs:import";
const XS_SIMPLE_TYPE: &str = "xs:simpleType";

#[derive(Debug, Clone)]
pub struct XsTypeNode<'a> {
    pub xsd_type: XsType,
    pub index: usize,
    pub id: String,
    pub name: String,
    pub node: &'a Element,
}

impl<'a> Display for XsTypeNode<'_> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}-{}-{}", self.index, self.id, self.name)
    }
}

fn parse_xs_info(node: &Element) -> Result<XsTypeNode<'_>, Error> {
    let index = COUNTER.fetch_add(1, Ordering::Relaxed);
    let xsd_type = XsType::parse(node)?;
    let name = match xsd_type {
        XsType::Annotation => format!("{}:{}", XS_ANNOTATION, index),
        XsType::Import => format!(
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
        XsType::Annotation | XsType::Import => name.clone(),
        _ => format!("{}:{}", node.fullname(), name),
    };
    Ok(XsTypeNode {
        xsd_type,
        index,
        id,
        name,
        node,
    })
}

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum XsType {
    Annotation,
    AttributeGroup,
    ComplexType,
    Element,
    Group,
    Import,
    SimpleType,
}

impl XsType {
    fn parse(node: &Element) -> Result<Self, Error> {
        let fullname = node.fullname();
        match fullname.as_str() {
            XS_ANNOTATION => Ok(XsType::Annotation),
            XS_ATTRIBUTE_GROUP => Ok(XsType::AttributeGroup),
            XS_COMPLEX_TYPE => Ok(XsType::ComplexType),
            XS_ELEMENT => Ok(XsType::Element),
            XS_GROUP => Ok(XsType::Group),
            XS_IMPORT => Ok(XsType::Import),
            XS_SIMPLE_TYPE => Ok(XsType::SimpleType),
            _ => Err(Error::UnknownSchemaNode(fullname)),
        }
    }
}

fn parse_type_definitions(
    xsd_nodes: &HashMap<String, XsTypeNode<'_>>,
    params: &XsdParserParams,
) -> Result<Vec<TypeDefinition>, Error> {
    let mut type_definitions = Vec::new();
    for x in xsd_nodes.values() {
        // handle the very special case of the dynamics `complexType`, transformed to an enum here.
        if let Some(dynamics) = parse_if_dynamics_complex_type(&x)? {
            type_definitions.push(TypeDefinition::Simple(SimpleType::Enum(dynamics)));
            continue;
        }
        if x.xsd_type != XsType::SimpleType {
            continue;
        }
        if is_enumeration_simple_type(x) {
            let eee = parse_enumeration(x)?;
            type_definitions.push(TypeDefinition::Simple(SimpleType::Enum(eee)));
        } else if let Some(pe) = params.pseudo_enums.get(&x.id) {
            let eee = Enumeration {
                index: x.index,
                id: x.id.clone(),
                name: x.name.clone(),
                base: find_base(&x)?,
                documentation: find_documentation(&x)?,
                members: pe.members.clone(),
                other_field: Some(MxEnumOption {
                    other_field_name: Symbol::new(pe.extra_field_name.clone()),
                    wrapper_class_name: Symbol::new(pe.class_name.clone()),
                    default_value: Symbol::new(pe.default_value.clone()),
                }),
            };
            type_definitions.push(TypeDefinition::Simple(SimpleType::Enum(eee)));
        }
    }
    Ok(type_definitions)
}

fn is_enumeration_simple_type(node: &XsTypeNode<'_>) -> bool {
    for child in node.node.children() {
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

fn parse_enumeration(node: &XsTypeNode<'_>) -> Result<Enumeration, Error> {
    let mut en = Enumeration::default();
    en.id = node.id.clone();
    en.name = node.name.clone();
    en.index = node.index;
    for child in node.node.children() {
        if child.fullname() == "xs:restriction" {
            if let Some(base) = child.attributes.map().get("base") {
                en.base = base.clone();
            } else {
                return Err(Error::SimpleTypeBaseNotFound);
            }
            for restriction in child.children() {
                if restriction.fullname() == "xs:enumeration" {
                    if let Some(value) = restriction.attributes.map().get("value") {
                        en.members.push(value.clone())
                    }
                } else {
                    return Err(Error::UnexpectedNode(restriction.fullname()));
                }
            }
        } else if child.fullname().as_str() == XS_ANNOTATION {
            let mut docsnodes = child.children();
            let documentation_node = docsnodes
                .next()
                .ok_or_else(|| Error::ExpectedOneChildOfXsAnnotation)?;
            if documentation_node.fullname().as_str() != "xs:documentation" {
                return Err(Error::ExpectedDocumentationNode);
            }
            en.documentation = documentation_node
                .text()
                .ok_or_else(|| Error::ExpectedDocumentationNodeToReturnText)?;
        }
    }
    Ok(en)
}

fn find_base(node: &XsTypeNode<'_>) -> Result<String, Error> {
    for child in node.node.children() {
        if child.fullname() == "xs:restriction" {
            if let Some(base) = child.attributes.map().get("base") {
                return Ok(base.clone());
            }
        }
    }
    Err(Error::SimpleTypeBaseNotFound)
}

fn find_documentation(node: &XsTypeNode<'_>) -> Result<String, Error> {
    let mut en = Enumeration::default();
    en.id = node.id.clone();
    en.name = node.name.clone();
    en.index = node.index;
    for child in node.node.children() {
        if child.fullname().as_str() == XS_ANNOTATION {
            let mut docsnodes = child.children();
            let documentation_node = docsnodes
                .next()
                .ok_or_else(|| Error::ExpectedOneChildOfXsAnnotation)?;
            if documentation_node.fullname().as_str() != "xs:documentation" {
                return Err(Error::ExpectedDocumentationNode);
            }
            return documentation_node
                .text()
                .ok_or_else(|| Error::ExpectedDocumentationNodeToReturnText);
        }
    }
    Ok("".to_owned())
}

fn parse_if_dynamics_complex_type(node: &XsTypeNode<'_>) -> Result<Option<Enumeration>, Error> {
    if node.xsd_type != ComplexType {
        return Ok(None);
    }
    if node.id.as_str() != "xs:complexType:dynamics" {
        return Ok(None);
    }
    let documentation = find_documentation(node)?;
    let mut members = Vec::new();
    for child in node.node.children() {
        if child.fullname() == XS_CHOICE {
            for member in child.children() {
                if member.fullname() == XS_ELEMENT {
                    let s = member
                        .attributes
                        .map()
                        .get("name")
                        .ok_or_else(|| Error::NameAttributeNotFound(node.id.clone()))?;
                    if s != "other-dynamics" {
                        members.push(s.to_owned());
                    }
                }
            }
        }
    }
    Ok(Some(Enumeration {
        index: node.index,
        id: "mx:customType:dynamics".to_string(),
        name: "dynamics-enum".to_string(),
        base: "xs:string".to_string(),
        documentation,
        members,
        other_field: Some(MxEnumOption {
            other_field_name: camel_case("other-dynamics"),
            wrapper_class_name: pascal_case("dynamics-value"),
            default_value: camel_case("other-dynamics"),
        }),
    }))
}
