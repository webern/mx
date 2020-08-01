use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::choice::Choice;
use crate::xsd::constants::{ANNOTATION, CHOICE, ELEMENT, GROUP, SEQUENCE};
use crate::xsd::element::Element;
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::sequence::Sequence;
use crate::xsd::{is_ref, name_attribute, ref_attribute, Occurs, Xsd};

#[derive(Clone, Debug)]
pub enum Group {
    Definition(GroupDefinition),
    Reference(GroupReference),
}

impl Group {
    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let (id, lineage) = Id::make(lineage, node)?;
        if is_ref(node) {
            Ok(Group::Reference(GroupReference::from_xml(node, lineage, xsd)?))
        } else {
            Ok(Group::Definition(GroupDefinition::from_xml(node, lineage, xsd)?))
        }
    }
}

#[derive(Clone, Debug)]
pub struct GroupDefinition {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub members: Vec<Member>,
}

#[derive(Clone, Debug)]
pub struct GroupReference {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub ref_: String,
    pub occurs: Occurs,
}

#[derive(Clone, Debug)]
pub enum Member {
    Choice(Choice),
    Element(Element),
    Sequence(Sequence),
}

impl GroupDefinition {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(GROUP, node, xsd)?;
        // way funky: The first match arm happens if it is a top-level root entry, otherwise the
        // caller has already created an ID for this and the second match arm is in effect.
        let (id, lineage) = match lineage {
            Lineage::Index(i) => Id::make(Lineage::Index(i), node)?,
            Lineage::Parent(p) => (p.clone(), Lineage::Parent(p.clone())),
        };
        let mut annotation = None;
        let mut members = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                CHOICE => {
                    let choice = Choice::from_xml(inner, lineage.clone(), xsd)?;
                    members.push(Member::Choice(choice));
                }
                ELEMENT => {
                    let element = Element::from_xml(inner, lineage.clone(), xsd)?;
                    members.push(Member::Element(element));
                }
                SEQUENCE => {
                    let sequence = Sequence::from_xml(inner, lineage.clone(), xsd)?;
                    members.push(Member::Sequence(sequence));
                }
                _ => return raise!("unsupported {} node, '{}'", GROUP, t),
            }
        }
        Ok(GroupDefinition { id, annotation, members })
    }
}

impl GroupReference {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        let id = lineage.parent().unwrap();
        if node.name.as_str() != GROUP {
            return raise!("expected '{}', got '{}'", GROUP, node.name.as_str());
        }
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                _ => return raise!("unsupported {} node, '{}'", GROUP, t),
            }
        }
        let ref_ = ref_attribute(node)?;
        Ok(GroupReference {
            id: id.clone(),
            annotation,
            ref_,
            occurs: Occurs::from_xml(node)?,
        })
    }
}

#[test]
fn parse_group_definition() {
    let xml_str = r#"
	<xs:group name="harmony-chord">
		<xs:annotation>
			<xs:documentation>blerp bloop bleep blop</xs:documentation>
		</xs:annotation>
		<xs:sequence>
			<xs:choice>
				<xs:element name="root" type="root"/>
				<xs:element name="function" type="style-text">
					<xs:annotation>
						<xs:documentation>mervin flervin</xs:documentation>
					</xs:annotation>
				</xs:element>
			</xs:choice>
			<xs:element name="kind" type="kind"/>
			<xs:element name="inversion" type="inversion" minOccurs="0"/>
			<xs:element name="bass" type="bass" minOccurs="0"/>
			<xs:element name="degree" type="degree" minOccurs="0" maxOccurs="unbounded"/>
		</xs:sequence>
	</xs:group>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let grp = Group::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    let grp = match grp {
        Group::Definition(def) => def,
        Group::Reference(_) => panic!("expected Definition, got Reference"),
    };
    assert_eq!(grp.id.index().unwrap(), want_index);
    let got_id = grp.id.to_string();
    let want_id = "group:harmony-chord".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = grp.documentation();
    let want_doc = "blerp bloop bleep blop";
    assert_eq!(got_doc, want_doc);
    assert_eq!(grp.members.len(), 1);
    let seq = grp.members.get(0).unwrap();
    match seq {
        Member::Choice(_) => panic!("expected Sequence, got Choice"),
        Member::Element(_) => panic!("expected Sequence, got Element"),
        Member::Sequence(s) => s,
    };
}

#[test]
fn parse_group_reference() {
    let xml_str = r#"<xs:group ref="non-traditional-key" minOccurs="0" maxOccurs="unbounded"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let grp = Group::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    let grp = match grp {
        Group::Definition(_) => panic!("expected Reference, got Definition"),
        Group::Reference(ref_) => ref_,
    };
    assert_eq!(grp.id.index().unwrap(), want_index);
    let got_id = grp.id.to_string();
    let want_id = "group:non-traditional-key".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = grp.documentation();
    let want_doc = "";
    assert_eq!(got_doc, want_doc);
    assert_eq!(grp.occurs.min_occurs, 0);
    assert!(grp.occurs.max_occurs.is_none());
}

#[test]
fn parse_group_reference_max_occurs() {
    let xml_str = r#"<xs:group ref="non-traditional-key" minOccurs="1" maxOccurs="8"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let grp = Group::from_xml(&xml, Lineage::Index(want_index), &Xsd::new("xs")).unwrap();
    let grp = match grp {
        Group::Definition(_) => panic!("expected Reference, got Definition"),
        Group::Reference(ref_) => ref_,
    };
    assert_eq!(grp.id.index().unwrap(), want_index);
    let got_id = grp.id.to_string();
    let want_id = "group:non-traditional-key".to_owned();
    assert_eq!(got_id, want_id);
    let got_doc = grp.documentation();
    let want_doc = "";
    assert_eq!(got_doc, want_doc);
    assert_eq!(grp.occurs.min_occurs, 1);
    assert_eq!(grp.occurs.max_occurs.unwrap(), 8);
}
