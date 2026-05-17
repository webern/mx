use crate::error::Result;

use crate::xsd::attribute::Attribute;
use crate::xsd::attribute_group::AttributeGroup;
use crate::xsd::constants::{ANNOTATION, ATTRIBUTE, ATTRIBUTE_GROUP};
use crate::xsd::id::Lineage;
use crate::xsd::Xsd;

pub type Attributes = Vec<AttributeItem>;

#[derive(Clone, Debug)]
pub enum AttributeItem {
    AttributeGroup(AttributeGroup),
    Attribute(Attribute),
}

impl AttributeItem {
    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let t = node.name.as_str();
        match t {
            ATTRIBUTE => Ok(Self::Attribute(Attribute::from_xml(node, lineage, xsd)?)),
            ATTRIBUTE_GROUP => Ok(Self::AttributeGroup(AttributeGroup::from_xml(
                node, lineage, xsd,
            )?)),
            _ => return raise!("unexpected node '{}'", t),
        }
    }
}

/// Ignores `parent` but parses each of its children into an AttributeItem. Ignores `annotation`
/// and returns an error if anything else other then `attribute` or `attributeGroup` is found.
pub fn add_attributes_from_xml(
    parent: &exile::Element,
    lineage: Lineage,
    xsd: &Xsd,
) -> Result<Attributes> {
    let mut items = Attributes::new();
    for it in parent.children() {
        let t = it.name.as_str();
        match t {
            ANNOTATION => continue,
            _ => items.push(AttributeItem::from_xml(it, lineage.clone(), xsd)?),
        }
    }
    Ok(items)
}

#[test]
fn parse_attribute_group() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let xml_str = r#"<xs:attributeGroup ref="document-attributes"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let result = AttributeItem::from_xml(&xml, Lineage::Parent(parent), &Xsd::new("xs")).unwrap();
    match &result {
        AttributeItem::AttributeGroup(x) => assert!(x.is_ref()),
        AttributeItem::Attribute(_) => panic!("expected AttributeGroup got Attribute"),
    }
}

#[test]
fn parse_attribute() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let xml_str = r#"<xs:attribute name="accelerate" type="yes-no"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let result = AttributeItem::from_xml(&xml, Lineage::Parent(parent), &Xsd::new("xs")).unwrap();
    match &result {
        AttributeItem::AttributeGroup(_) => panic!("expected AttributeGroup got Attribute"),
        AttributeItem::Attribute(a) => assert_eq!(a.name, "accelerate"),
    }
}

#[test]
fn attributes() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let xml_str = r#"
    	<xs:attributeGroup name="printout">
		<xs:annotation>
			<xs:documentation>Flerubin.</xs:documentation>
		</xs:annotation>
		<xs:attributeGroup ref="print-object"/>
		<xs:attribute name="print-dot" type="yes-no"/>
		<xs:attributeGroup ref="print-spacing"/>
		<xs:attribute name="print-lyric" type="yes-no"/>
	</xs:attributeGroup>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let result = add_attributes_from_xml(&xml, Lineage::Parent(parent), &Xsd::new("xs")).unwrap();
    assert_eq!(result.len(), 4);
    match result.get(0).unwrap() {
        AttributeItem::AttributeGroup(ag) => assert!(ag.is_ref()),
        AttributeItem::Attribute(_) => panic!("wrong variant"),
    }
    match result.get(1).unwrap() {
        AttributeItem::AttributeGroup(_) => panic!("wrong variant"),
        AttributeItem::Attribute(a) => assert!(a.defined_by.is_type()),
    }
}
