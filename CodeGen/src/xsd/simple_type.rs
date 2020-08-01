use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, LIST, NAME, RESTRICTION, SIMPLE_TYPE, UNION};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::list::List;
use crate::xsd::restriction::Restriction;
use crate::xsd::union::Union;
use crate::xsd::{name_attribute, Xsd};

#[derive(Clone, Debug)]
pub struct SimpleType {
    pub id: Id,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

#[derive(Clone, Debug)]
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(SIMPLE_TYPE, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            payload = match t {
                ANNOTATION => {
                    annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?);
                    continue;
                }
                RESTRICTION => Some(Payload::Restriction(Restriction::from_xml(inner, lineage.clone(), xsd)?)),
                LIST => Some(Payload::List(List::from_xml(inner, lineage.clone(), xsd)?)),
                UNION => Some(Payload::Union(Union::from_xml(inner, lineage.clone(), xsd)?)),
                _ => {
                    return raise!("unexpected element name '{}'", t);
                }
            };
        }
        let payload = payload.ok_or(make_err!("{} is incomplete", SIMPLE_TYPE))?;
        Ok(SimpleType {
            id,
            name: name_attribute(node)?,
            annotation,
            payload,
        })
    }
}

#[test]
fn parse_enum() {
    use super::restriction::Facet;
    use crate::xsd::constants::*;
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
    let st = SimpleType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(st.id.index().unwrap(), want_index);
    let got_id = st.id.to_string();
    let want_id = "simpleType:above-below".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = st.documentation();
    let want_doc = "The above-below type is used to indicate whether one element appears above or below another element.";
    assert_eq!(got_doc, want_doc);
    match st.payload {
        Payload::Restriction(r) => {
            assert_eq!(r.facets.len(), 2);
            match r.facets.get(0).unwrap() {
                Facet::Enumeration(value) => {
                    assert_eq!(value.as_str(), "above");
                }
                _ => panic!("expected '{}'", ENUMERATION),
            }
            match r.facets.get(1).unwrap() {
                Facet::Enumeration(value) => {
                    assert_eq!(value.as_str(), "below");
                }
                _ => panic!("expected '{}'", ENUMERATION),
            }
        }
        _ => panic!("expected '{}'", RESTRICTION),
    }
}

#[test]
fn parse_numeric() {
    use super::restriction::{Facet, Number};
    use crate::xsd::constants::*;
    let xml_str = r#"
	<xs:simpleType name="midi-16">
		<xs:annotation>
			<xs:documentation>The midi-16 type is used to express MIDI 1.0 values that range from 1 to 16.</xs:documentation>
		</xs:annotation>
		<xs:restriction base="xs:positiveInteger">
			<xs:minInclusive value="1"/>
			<xs:maxInclusive value="16"/>
		</xs:restriction>
	</xs:simpleType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 4;
    let st = SimpleType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(st.id.index().unwrap(), want_index);
    let got_id = st.id.to_string();
    let want_id = "simpleType:midi-16".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = st.documentation();
    let want_doc = "The midi-16 type is used to express MIDI 1.0 values that range from 1 to 16.";
    assert_eq!(got_doc, want_doc);
    match st.payload {
        Payload::Restriction(r) => {
            assert_eq!(r.facets.len(), 2);
            match r.facets.get(0).unwrap() {
                Facet::MinInclusive(number) => match number {
                    Number::Integer(i) => assert_eq!(1, *i),
                    _ => panic!("expected integer"),
                },
                _ => panic!("expected '{}'", MIN_INCLUSIVE),
            }
            match r.facets.get(1).unwrap() {
                Facet::MaxInclusive(number) => match number {
                    Number::Integer(i) => assert_eq!(16, *i),
                    _ => panic!("expected integer"),
                },
                _ => panic!("expected '{}'", MAX_INCLUSIVE),
            }
        }
        _ => panic!("expected '{}'", RESTRICTION),
    }
}

#[test]
fn parse_pattern() {
    use super::restriction::Facet;
    use crate::xsd::constants::*;
    let xml_str = r#"
	<xs:simpleType name="time-only">
		<xs:annotation>
			<xs:documentation>blerp</xs:documentation>
		</xs:annotation>
		<xs:restriction base="xs:token">
			<xs:pattern value="[1-9][0-9]*(, ?[1-9][0-9]*)*"/>
		</xs:restriction>
	</xs:simpleType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let st = SimpleType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(st.id.index().unwrap(), want_index);
    let got_id = st.id.to_string();
    let want_id = "simpleType:time-only".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = st.documentation();
    let want_doc = "blerp";
    assert_eq!(got_doc, want_doc);
    match st.payload {
        Payload::Restriction(r) => {
            assert_eq!(r.facets.len(), 1);
            match r.facets.get(0).unwrap() {
                Facet::Pattern(value) => {
                    assert_eq!(value.as_str(), "[1-9][0-9]*(, ?[1-9][0-9]*)*");
                }
                _ => panic!("expected '{}'", PATTERN),
            }
        }
        _ => panic!("expected '{}'", RESTRICTION),
    }
}
