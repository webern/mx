use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::{
    name_attribute, ref_attribute, type_attribute, EntryType, ANNOTATION, ATTRIBUTE_GROUP, ID,
};
use std::convert::TryInto;

pub const ATTRIBUTE: &str = "attribute";

pub struct AttributeGroup {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub members: Vec<Member>,
}

pub struct AttributeRef {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub type_: String,
}

pub struct AttributeGroupRef {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub ref_: String,
}

pub enum Member {
    AttributeRef(AttributeRef),
    AttributeGroupRef(AttributeGroupRef),
}

impl AttributeGroup {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != ATTRIBUTE_GROUP {
            return raise!(
                "expected '{}', got '{}'",
                ATTRIBUTE_GROUP,
                node.name.as_str()
            );
        }
        let mut annotation = None;
        let mut members = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => {
                    annotation = Some(Annotation::from_xml(inner, index)?);
                }
                ATTRIBUTE => {
                    let name = name_attribute(inner)?;
                    members.push(Member::AttributeRef(AttributeRef {
                        id: ID {
                            entry_type: EntryType::Other(ATTRIBUTE.into()),
                            name: name.clone(),
                        },
                        index,
                        name,
                        type_: type_attribute(inner)?,
                    }))
                }
                ATTRIBUTE_GROUP => {
                    let name = name_attribute(inner)?;
                    members.push(Member::AttributeGroupRef(AttributeGroupRef {
                        id: ID {
                            entry_type: EntryType::Other(ATTRIBUTE.into()),
                            name: name.clone(),
                        },
                        index,
                        name,
                        ref_: ref_attribute(inner)?,
                    }))
                }
                _ => return raise!("unexpected node '{}'", t),
            }
        }
        let id = ID {
            entry_type: EntryType::AttributeGroup,
            name: name_attribute(node)?,
        };
        Ok(AttributeGroup {
            id,
            index,
            annotation,
            members,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
    	<xs:attributeGroup name="bend-sound">
		<xs:annotation>
			<xs:documentation>blargh</xs:documentation>
		</xs:annotation>
		<xs:attribute name="accelerate" type="yes-no"/>
		<xs:attribute name="beats" type="trill-beats"/>
		<xs:attribute name="first-beat" type="percent"/>
		<xs:attribute name="last-beat" type="percent"/>
	</xs:attributeGroup>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let ag = AttributeGroup::from_xml(&xml, want_index).unwrap();
    assert_eq!(ag.index, want_index);
    let got_id = ag.id.to_string();
    let want_id = "bend-sound (attributeGroup)".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = ag.documentation();
    let want_doc = "blargh";
    assert_eq!(got_doc, want_doc);
    assert_eq!(ag.members.len(), 4);
    unimplemented!();
}
