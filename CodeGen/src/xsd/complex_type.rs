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
use crate::xsd::id::{Id, Lineage, RootNodeId, RootNodeType};
use crate::xsd::sequence::Sequence;
use crate::xsd::simple_content::SimpleContent;
use crate::xsd::Xsd;

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ComplexType {
    pub id: Id,
    pub name: String,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub enum Payload {
    ComplexContent(ComplexContent),
    SimpleContent(SimpleContent),
    Parent(Parent),
    None,
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub enum Children {
    Choice(Choice),
    Group(Group),
    Sequence(Sequence),
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(COMPLEX_TYPE, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut payload = Payload::None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                CHOICE | GROUP | SEQUENCE => {
                    payload = Payload::Parent(Parent::from_xml(node, lineage.clone(), xsd)?);
                    break;
                }
                COMPLEX_CONTENT => {
                    payload = Payload::ComplexContent(ComplexContent::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
                    )?)
                }
                SIMPLE_CONTENT => {
                    payload = Payload::SimpleContent(SimpleContent::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
                    )?)
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
        Ok(ComplexType {
            id,
            name,
            annotation,
            payload,
        })
    }
}

impl Parent {
    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let mut parent = Parent {
            attributes: Attributes::new(),
            children: None,
        };
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                CHOICE => {
                    parent.children = Some(Children::Choice(Choice::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
                    )?));
                }
                GROUP => {
                    parent.children = Some(Children::Group(Group::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
                    )?));
                }
                SEQUENCE => {
                    parent.children = Some(Children::Sequence(Sequence::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
                    )?));
                }
                ATTRIBUTE | ATTRIBUTE_GROUP => {
                    parent
                        .attributes
                        .push(AttributeItem::from_xml(inner, lineage.clone(), xsd)?)
                }
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
    let ct = ComplexType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(format!("{}", ct.id), "complexType:14524941899531583637");
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
    let ct = ComplexType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(format!("{}", ct.id), "complexType:system-margins");
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
    let ct = ComplexType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(format!("{}", ct.id), "complexType:arrow");
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
    let ct = ComplexType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(format!("{}", ct.id), "complexType:heel-toe");
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
    let ct = ComplexType::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    assert_eq!(format!("{}", ct.id), "complexType:hole-closed");
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
