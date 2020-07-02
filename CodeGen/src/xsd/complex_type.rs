use crate::error::Result;
use crate::xsd::annotation::Annotation;

use crate::xsd::attribute_group::AttributeGroup;
use crate::xsd::attributes::{AttributeItem, Attributes};
use crate::xsd::choice::Choice;
use crate::xsd::complex_content::ComplexContent;
use crate::xsd::constants::{
    ANNOTATION, ATTRIBUTE, ATTRIBUTE_GROUP, CHOICE, COMPLEX_CONTENT, COMPLEX_TYPE, GROUP, NAME,
    SEQUENCE, SIMPLE_CONTENT,
};
use crate::xsd::group::Group;
use crate::xsd::sequence::Sequence;
use crate::xsd::simple_content::SimpleContent;
use crate::xsd::{EntryType, ID};

pub struct ComplexType {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

pub enum Payload {
    ComplexContent(ComplexContent),
    SimpleContent(SimpleContent),
    Parent(Parent),
}

pub enum Children {
    Choice(Choice),
    Group(Group),
    Sequence(Sequence),
}

pub struct Parent {
    attributes: Attributes,
    children: Option<Children>,
}

impl ComplexType {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != COMPLEX_TYPE {
            return raise!("expected '{}', got '{}'", COMPLEX_TYPE, node.name.as_str());
        }
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                CHOICE | GROUP | SEQUENCE => {
                    payload = Some(Payload::Parent(Parent::from_xml(node, index)?));
                    break;
                }
                COMPLEX_CONTENT => {
                    payload = Some(Payload::ComplexContent(ComplexContent::from_xml(
                        node, index,
                    )?))
                }
                SIMPLE_CONTENT => {
                    payload = Some(Payload::SimpleContent(SimpleContent::from_xml(
                        node, index,
                    )?))
                }
                _ => return raise!("unexpected node '{}' while parsing complexType", t),
            }
        }
        let name = if let Some(n) = node.attributes.map().get(NAME) {
            n.clone()
        } else {
            "".to_owned()
        };
        let id = ID {
            entry_type: EntryType::ComplexType,
            name: if name.is_empty() {
                format!("{}", index)
            } else {
                name.clone()
            },
        };
        let payload = if let Some(p) = payload {
            p
        } else {
            return raise!("could not parse the complexType");
        };
        Ok(ComplexType {
            id,
            index,
            name,
            annotation,
            payload,
        })
    }
}

impl Parent {
    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let mut parent = Parent {
            attributes: vec![],
            children: None,
        };
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                CHOICE => {
                    parent.children = Some(Children::Choice(Choice::from_xml(inner, index)?));
                }
                GROUP => {
                    parent.children = Some(Children::Group(Group::from_xml(inner, index)?));
                }
                SEQUENCE => {
                    parent.children = Some(Children::Sequence(Sequence::from_xml(inner, index)?));
                }
                ATTRIBUTE | ATTRIBUTE_GROUP => parent
                    .attributes
                    .push(AttributeItem::from_xml(inner, index)?),
                _ => return raise!("unable to parse complexType, unexpected node '{}'", t),
            }
        }
        Ok(parent)
    }
}

#[test]
fn parse_parent() {
    let xml_str = r#"
    <xs:complexType>
		<xs:sequence>
			<xs:group ref="score-header"/>
			<xs:element name="part" maxOccurs="unbounded">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="measure" maxOccurs="unbounded">
							<xs:complexType>
								<xs:group ref="music-data"/>
								<xs:attributeGroup ref="measure-attributes"/>
							</xs:complexType>
						</xs:element>
					</xs:sequence>
					<xs:attributeGroup ref="part-attributes"/>
				</xs:complexType>
			</xs:element>
		</xs:sequence>
		<xs:attributeGroup ref="document-attributes"/>
	</xs:complexType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 6;
    let ct = ComplexType::from_xml(&xml, want_index).unwrap();
    assert_eq!(format!("{}", ct.id), "6 (complexType)");
    assert_eq!(ct.documentation().as_str(), "");
    let parent = if let Payload::Parent(p) = ct.payload {
        p
    } else {
        panic!("wrong payload type");
    };
    assert_eq!(parent.attributes.len(), 1);
    match parent.attributes.get(0).unwrap() {
        AttributeItem::AttributeGroup(ag) => match ag {
            AttributeGroup::Def(_) => panic!("expected Ref got Def"),
            AttributeGroup::Ref(s) => {
                assert_eq!(s.ref_.as_str(), "document-attributes");
            }
        },
        AttributeItem::Attribute(_) => panic!("expected AttributeGroup got Attribute"),
    }
    match parent.children.unwrap() {
        Children::Choice(_) => panic!("want Sequence got Choice"),
        Children::Group(_) => panic!("want Sequence got Group"),
        Children::Sequence(_) => {}
    }
}
