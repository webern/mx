use crate::error::Result;
use crate::xsd::annotation::Annotation;
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

#[derive(Clone, Debug)]
pub struct ComplexType {
    pub id: ID,
    pub index: u64,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

#[derive(Clone, Debug)]
pub enum Payload {
    ComplexContent(ComplexContent),
    SimpleContent(SimpleContent),
    Parent(Parent),
    None,
}

#[derive(Clone, Debug)]
pub enum Children {
    Choice(Choice),
    Group(Group),
    Sequence(Sequence),
}

#[derive(Clone, Debug)]
pub struct Parent {
    pub attributes: Attributes,
    pub children: Option<Children>,
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
        let mut payload = Payload::None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                CHOICE | GROUP | SEQUENCE => {
                    payload = Payload::Parent(Parent::from_xml(node, index)?);
                    break;
                }
                COMPLEX_CONTENT => {
                    payload = Payload::ComplexContent(ComplexContent::from_xml(inner, index)?)
                }
                SIMPLE_CONTENT => {
                    payload = Payload::SimpleContent(SimpleContent::from_xml(inner, index)?)
                }
                ATTRIBUTE | ATTRIBUTE_GROUP => { /* will be parsed by Parent::from_xml() */ }
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
                ANNOTATION => { /* ignore because it's parsed by ComplexType::from_xml */ }
                _ => return raise!("unable to parse complexType, unexpected node '{}'", t),
            }
        }
        Ok(parent)
    }
}

#[test]
fn parse_parent_sequence() {
    use crate::xsd::attribute_group::AttributeGroup;
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

#[test]
fn parse_parent_group() {
    let xml_str = r#"
    <xs:complexType name="system-margins">
		<xs:annotation>
			<xs:documentation>System margins are relative to the page margins.</xs:documentation>
		</xs:annotation>
		<xs:group ref="left-right-margins"/>
	</xs:complexType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 6;
    let ct = ComplexType::from_xml(&xml, want_index).unwrap();
    assert_eq!(format!("{}", ct.id), "system-margins (complexType)");
    assert_eq!(
        ct.documentation().as_str(),
        "System margins are relative to the page margins."
    );
    let parent = if let Payload::Parent(p) = ct.payload {
        p
    } else {
        panic!("wrong payload type");
    };
    assert_eq!(parent.attributes.len(), 0);
    match parent.children.unwrap() {
        Children::Choice(_) => panic!("want Group got Choice"),
        Children::Group(_) => {}
        Children::Sequence(_) => panic!("want Group got Sequence"),
    }
}

#[test]
fn parse_parent_choice() {
    use crate::xsd::attribute_group::AttributeGroup;
    let xml_str = r#"
	<xs:complexType name="arrow">
		<xs:annotation>
			<xs:documentation>The arrow element represents an arrow.</xs:documentation>
		</xs:annotation>
		<xs:choice>
			<xs:sequence>
				<xs:element name="arrow-direction" type="arrow-direction"/>
				<xs:element name="arrow-style" type="arrow-style" minOccurs="0"/>
			</xs:sequence>
			<xs:element name="circular-arrow" type="circular-arrow"/>
		</xs:choice>
		<xs:attributeGroup ref="print-style"/>
		<xs:attributeGroup ref="placement"/>
	</xs:complexType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 6;
    let ct = ComplexType::from_xml(&xml, want_index).unwrap();
    assert_eq!(format!("{}", ct.id), "arrow (complexType)");
    assert_eq!(
        ct.documentation().as_str(),
        "The arrow element represents an arrow."
    );
    let parent = if let Payload::Parent(p) = ct.payload {
        p
    } else {
        panic!("wrong payload type");
    };
    assert_eq!(parent.attributes.len(), 2);
    match parent.attributes.get(0).unwrap() {
        AttributeItem::AttributeGroup(ag) => match ag {
            AttributeGroup::Def(_) => panic!("expected Ref got Def"),
            AttributeGroup::Ref(s) => {
                assert_eq!(s.ref_.as_str(), "print-style");
            }
        },
        AttributeItem::Attribute(_) => panic!("expected AttributeGroup got Attribute"),
    }
    match parent.attributes.get(1).unwrap() {
        AttributeItem::AttributeGroup(ag) => match ag {
            AttributeGroup::Def(_) => panic!("expected Ref got Def"),
            AttributeGroup::Ref(s) => {
                assert_eq!(s.ref_.as_str(), "placement");
            }
        },
        AttributeItem::Attribute(_) => panic!("expected AttributeGroup got Attribute"),
    }
    match parent.children.unwrap() {
        Children::Choice(_) => {}
        Children::Group(_) => panic!("want Choice got Group"),
        Children::Sequence(_) => panic!("want Choice got Sequence"),
    }
}

#[test]
fn parse_complex_content() {
    let xml_str = r#"
   <xs:complexType name="heel-toe">
       <xs:annotation>
           <xs:documentation>Heel and toe elements are used with organ pedals.</xs:documentation>
       </xs:annotation>
       <xs:complexContent>
           <xs:extension base="empty-placement">
               <xs:attribute name="substitution" type="yes-no"/>
           </xs:extension>
       </xs:complexContent>
   </xs:complexType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 6;
    let ct = ComplexType::from_xml(&xml, want_index).unwrap();
    assert_eq!(format!("{}", ct.id), "heel-toe (complexType)");
    assert_eq!(
        ct.documentation().as_str(),
        "Heel and toe elements are used with organ pedals."
    );
    let cc = if let Payload::ComplexContent(cc) = ct.payload {
        cc
    } else {
        panic!("wrong payload type");
    };
    assert_eq!(cc.extension.attributes.len(), 1);
    match cc.extension.attributes.get(0).unwrap() {
        AttributeItem::Attribute(a) => {
            assert_eq!(a.name.as_str(), "substitution");
            assert!(a.defined_by.is_type());
            assert_eq!(a.defined_by.value(), "yes-no");
        }
        AttributeItem::AttributeGroup(_) => panic!("expected Attribute got AttributeGroup"),
    }
}

#[test]
fn parse_simple_content() {
    use super::simple_content;
    let xml_str = r#"
    <xs:complexType name="hole-closed">
        <xs:annotation>
            <xs:documentation>The hole-closed type represents whether the...</xs:documentation>
        </xs:annotation>
        <xs:simpleContent>
            <xs:extension base="hole-closed-value">
                <xs:attribute name="location" type="hole-closed-location"/>
            </xs:extension>
        </xs:simpleContent>
    </xs:complexType>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 6;
    let ct = ComplexType::from_xml(&xml, want_index).unwrap();
    assert_eq!(format!("{}", ct.id), "hole-closed (complexType)");
    assert_eq!(
        ct.documentation().as_str(),
        "The hole-closed type represents whether the..."
    );
    let sc = if let Payload::SimpleContent(sc) = ct.payload {
        sc
    } else {
        panic!("wrong payload type");
    };
    let ext = match &sc.payload {
        simple_content::Payload::Extension(x) => x,
    };
    assert_eq!(ext.base.as_str(), "hole-closed-value");
    assert_eq!(ext.attributes.len(), 1);
    match ext.attributes.get(0).unwrap() {
        AttributeItem::AttributeGroup(_) => panic!("expected Attribute got AttributeGroup"),
        AttributeItem::Attribute(a) => {
            assert!(a.defined_by.is_type());
            assert_eq!(a.defined_by.value(), "hole-closed-location");
        }
    }
}
