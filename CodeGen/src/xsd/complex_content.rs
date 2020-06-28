use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::attribute_group::{parse_attribute_group_ref, ATTRIBUTE};
use crate::xsd::attribute_group::{
    parse_attribute_ref, parse_attribute_type, AttributeGroupRef, AttributeRef, AttributeType,
};
use crate::xsd::extension::{Extension, EXTENSION};
use crate::xsd::{base_attribute, EntryType, ANNOTATION, ID, IMPORT, REF};
use std::convert::TryInto;

pub struct ComplexContent {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

pub enum Payload {
    Extension(Extension),
}

pub const COMPLEX_CONTENT: &str = "complexContent";

impl ComplexContent {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != COMPLEX_CONTENT {
            return raise!("expected '{}', got '{}'", COMPLEX_CONTENT, &node.name);
        }
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                EXTENSION => {
                    payload = Some(Payload::Extension(Extension::from_xml(inner, index)?));
                }
                _ => return raise!("unsupported simpleContent node '{}'", t),
            }
        }
        let id = ID {
            entry_type: EntryType::Other(COMPLEX_CONTENT.to_owned()),
            name: format!("{}", index),
        };
        let payload = if let Some(p) = payload {
            p
        } else {
            return raise!("{} payload was not found", COMPLEX_CONTENT);
        };
        Ok(ComplexContent {
            id,
            index,
            annotation,
            payload,
        })
    }
}

#[test]
fn parse() {
    use super::extension::Member;
    let xml_str = r#"
		<xs:complexContent>
			<xs:extension base="time-modification">
				<xs:attribute name="type" type="start-stop" use="required"/>
				<xs:attribute name="bracket" type="yes-no"/>
				<xs:attribute name="show-number" type="show-tuplet"/>
			</xs:extension>
		</xs:complexContent>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 1;
    let want_id = "1 (complexContent)".to_owned();
    let want_doc = "";
    let sc = ComplexContent::from_xml(&xml, want_index).unwrap();
    let got_doc = sc.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = sc.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", sc.id);
    assert_eq!(got_id, want_id);
    let got_type = sc.id.entry_type;
    assert_eq!(got_type, EntryType::Other(COMPLEX_CONTENT.to_owned()));
    match sc.payload {
        Payload::Extension(ext) => {
            assert_eq!(ext.base.as_str(), "time-modification");
            assert_eq!(ext.members.len(), 3);
            let member = ext.members.get(0).unwrap();
            match member {
                Member::AttributeType(x) => {
                    assert_eq!(x.name.as_str(), "type");
                    assert_eq!(x.type_.as_str(), "start-stop");
                    assert!(x.required);
                }
                Member::AttributeRef(_) => {
                    panic!("expected 'AttributeType' but got 'AttributeRef'")
                }
                Member::AttributeGroupRef(_) => {
                    panic!("expected 'AttributeType' but got 'AttributeGroupRef'")
                }
            }
        }
    }
}
