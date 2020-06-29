use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::choice::Choice;
use crate::xsd::constants::{ANNOTATION, CHOICE, ELEMENT, GROUP, SEQUENCE};
use crate::xsd::element::Element;
use crate::xsd::group::Group;
use crate::xsd::{name_attribute, EntryType, Occurs, ID};
use std::convert::TryInto;

pub struct Sequence {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub occurs: Occurs,
    pub members: Vec<Member>,
}

pub enum Member {
    Choice(Choice),
    Element(Element),
    Group(Group),
    Sequence(Sequence),
}

impl Sequence {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != SEQUENCE {
            return raise!("expected '{}', got '{}'", SEQUENCE, node.name.as_str());
        }
        let mut annotation = None;
        let mut members = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                CHOICE => members.push(Member::Choice(Choice::from_xml(inner, index)?)),
                ELEMENT => members.push(Member::Element(Element::from_xml(inner, index)?)),
                GROUP => members.push(Member::Group(Group::from_xml(inner, index)?)),
                SEQUENCE => members.push(Member::Sequence(Sequence::from_xml(inner, index)?)),
                _ => return raise!("unknown {} member: '{}'", SEQUENCE, t),
            }
        }
        let id = ID {
            entry_type: EntryType::Other(SEQUENCE.to_owned()),
            name: format!("{}", index),
        };
        Ok(Sequence {
            id,
            index,
            annotation,
            occurs: Occurs::from_xml(node)?,
            members,
        })
    }
}

#[test]
fn parse() {
    use crate::xsd::constants::*;
    let xml_str = r#"
    <xs:sequence>
		<xs:element name="identification" type="identification" minOccurs="0"/>
		<xs:element name="part-name" type="part-name"/>
		<xs:element name="part-name-display" type="name-display" minOccurs="0"/>
		<xs:element name="part-abbreviation" type="part-name" minOccurs="0"/>
		<xs:element name="part-abbreviation-display" type="name-display" minOccurs="0"/>
		<xs:element name="group" type="xs:string" minOccurs="0" maxOccurs="unbounded">
			<xs:annotation>
				<xs:documentation>flerp floop fleep flop</xs:documentation>
			</xs:annotation>
		</xs:element>
		<xs:element name="score-instrument" type="score-instrument" minOccurs="0" maxOccurs="unbounded"/>
		<xs:sequence minOccurs="0" maxOccurs="unbounded">
			<xs:element name="midi-device" type="midi-device" minOccurs="0"/>
			<xs:element name="midi-instrument" type="midi-instrument" minOccurs="0"/>
		</xs:sequence>
	</xs:sequence>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let seq = Sequence::from_xml(&xml, want_index).unwrap();
    let got_id = format!("{}", seq.id);
    let want_id = "3 (sequence)";
    assert_eq!(got_id.as_str(), want_id);
    let got_doc = seq.documentation();
    let want_doc = "";
    assert_eq!(got_doc, want_doc);
    assert_eq!(
        seq.occurs,
        Occurs {
            min_occurs: 1,
            max_occurs: Some(1),
        }
    );
    assert_eq!(seq.members.len(), 8);
    match seq.members.get(5).unwrap() {
        Member::Choice(_) => panic!("expecting '{}', got '{}'", "Element", "Choice"),
        Member::Element(_) => {}
        Member::Group(_) => panic!("expecting '{}', got '{}'", "Element", "Group"),
        Member::Sequence(_) => panic!("expecting '{}', got '{}'", "Element", "Sequence"),
    };
    match seq.members.get(7).unwrap() {
        Member::Choice(_) => panic!("expecting '{}', got '{}'", "Sequence", "Choice"),
        Member::Element(e) => panic!("expecting '{}', got '{}'", "Sequence", "Element"),
        Member::Group(_) => panic!("expecting '{}', got '{}'", "Sequence", "Group"),
        Member::Sequence(_) => {}
    };
}
