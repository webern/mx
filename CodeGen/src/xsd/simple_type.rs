use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::list::List;
use crate::xsd::restriction::Restriction;
use crate::xsd::union::Union;
use crate::xsd::{EntryType, ANNOTATION, ID, SIMPLE_TYPE};
use std::convert::TryInto;

pub struct SimpleType {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

pub const BASE: &str = "base";
pub const LIST: &str = "list";
pub const RESTRICTION: &str = "restriction";
pub const UNION: &str = "union";

pub enum Payload {
    Restriction(Restriction),
    List(List),
    Union(Union),
}

impl SimpleType {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        "".to_owned()
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        // let mut items = Vec::new();
        if node.name.as_str() != SIMPLE_TYPE {
            return raise!("expected '{}', got '{}'", SIMPLE_TYPE, &node.name);
        }
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            let payload = match t {
                ANNOTATION => {
                    annotation = Some(Annotation::from_xml(inner, index)?);
                    continue;
                }
                RESTRICTION => Some(Payload::Restriction(Restriction::from_xml(inner, index)?)),
                LIST => Some(Payload::List(List::from_xml(inner, index)?)),
                UNION => Some(Payload::Union(Union::from_xml(inner, index)?)),
                _ => {
                    return raise!("unexpected element name '{}'", t);
                }
            };
        }
        let id = ID {
            entry_type: EntryType::Annotation,
            name: format!("{}", index),
        };
        let payload = payload.ok_or(make_err!("{} is incomplete", SIMPLE_TYPE))?;
        Ok(SimpleType {
            id,
            index,
            annotation,
            payload,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
	<xs:simpleType name="above-below">
		<xs:annotation>
			<xs:documentation>The above-below type is used to indicate whether one element appears above or below another element.</xs:documentation>
		</xs:annotation>
		<xs:restriction base="xs:token">
			<xs:enumeration value="above"/>
			<xs:enumeration value="below"/>
		</xs:restriction>
	</xs:simpleType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let st = SimpleType::from_xml(&xml, want_index).unwrap();
    assert_eq!(st.index, want_index);
    let got_id = st.id.to_string();
    let want_id = "above-below (simpleType)".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = st.documentation();
    let want_doc = "The above-below type is used to indicate whether one element appears above or below another element.";
    assert_eq!(got_doc, want_doc);
}
