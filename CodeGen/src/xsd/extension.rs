use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::attribute_group::{parse_attribute_group_ref, ATTRIBUTE};
use crate::xsd::attribute_group::{
    parse_attribute_ref, parse_attribute_type, AttributeGroupRef, AttributeRef, AttributeType,
};
use crate::xsd::{base_attribute, EntryType, ANNOTATION, ID, IMPORT, REF};
use std::convert::TryInto;

pub struct Extension {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub base: String,
    pub members: Vec<Member>,
}

pub enum Member {
    AttributeType(AttributeType),
    AttributeRef(AttributeRef),
    AttributeGroupRef(AttributeGroupRef),
}

pub const EXTENSION: &str = "extension";

impl Extension {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != EXTENSION {
            return raise!("expected '{}', got '{}'", EXTENSION, &node.name);
        }
        let base = base_attribute(node)?;
        let mut annotation = None;
        let mut members = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                ATTRIBUTE => {
                    if let Some(_) = inner.attributes.map().get(REF) {
                        members.push(Member::AttributeRef(parse_attribute_ref(inner, index)?));
                    } else {
                        members.push(Member::AttributeType(parse_attribute_type(inner, index)?));
                    }
                }
                ATTRIBUTE_GROUP => {
                    members.push(Member::AttributeGroupRef(parse_attribute_group_ref(
                        inner, index,
                    )?));
                }
            }
        }
        let id = ID {
            entry_type: EntryType::Other(EXTENSION.to_owned()),
            name: format!("{}", index),
        };
        Ok(Extension {
            id,
            index,
            annotation,
            base,
            members,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
	<xs:extension base="xs:string">
		<xs:attribute name="type" type="start-stop" use="required"/>
		<xs:attribute name="symbol" type="principal-voice-symbol" use="required"/>
		<xs:attributeGroup ref="print-style-align"/>
	</xs:extension>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 989898;
    let want_id = "989898 (extension)".to_owned();
    let want_doc = "";
    let ext = Extension::from_xml(&xml, want_index).unwrap();
    let got_doc = ext.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = ext.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", ext.id);
    assert_eq!(got_id, want_id);
    let got_type = ext.id.entry_type;
    assert_eq!(got_type, EntryType::Other(EXTENSION.to_owned()));
    let got_base = ext.base.as_str();
    let want_base = "xs:string";
    assert_eq!(got_base, want_base);
    assert_eq!(ext.members.len(), 3);
    let member = ext.members.get(0).unwrap();
    match member {
        Member::AttributeType(x) => {
            assert_eq!(x.name.as_str(), "type");
            assert_eq!(x.type_.as_str(), "start-stop");
            assert!(x.required);
        }
        Member::AttributeRef(_) => panic!("expected 'AttributeType' but got 'AttributeRef'"),
        Member::AttributeGroupRef(_) => {
            panic!("expected 'AttributeType' but got 'AttributeGroupRef'")
        }
    }
    let member = ext.members.get(2).unwrap();
    match member {
        Member::AttributeType(_) => panic!("expected 'AttributeGroupRef' but got 'AttributeType'"),
        Member::AttributeRef(_) => panic!("expected 'AttributeGroupRef' but got 'AttributeRef'"),
        Member::AttributeGroupRef(x) => {
            assert_eq!(x.ref_.as_str(), "print-style-align");
        }
    }
}
