use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::constants::{ANNOTATION, COMPLEX_CONTENT, EXTENSION};
use crate::xsd::extension::Extension;
use crate::xsd::{base_attribute, EntryType, ID};
use std::convert::TryInto;

pub struct ComplexContent {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub extension: Extension,
}

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
        let mut extension = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                EXTENSION => {
                    extension = Some(Extension::from_xml(inner, index)?);
                }
                _ => return raise!("unsupported simpleContent node '{}'", t),
            }
        }
        let id = ID {
            entry_type: EntryType::Other(COMPLEX_CONTENT.to_owned()),
            name: format!("{}", index),
        };
        let extension = if let Some(ext) = extension {
            ext
        } else {
            return raise!("{} payload was not found", COMPLEX_CONTENT);
        };
        Ok(ComplexContent {
            id,
            index,
            annotation,
            extension,
        })
    }
}

#[test]
fn parse() {
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
    assert_eq!(extension.base.as_str(), "time-modification");
    assert_eq!(extension.members.len(), 3);
    let member = extension.members.get(0).unwrap();
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
}
