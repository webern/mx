use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, CHOICE, ELEMENT, GROUP, NAME, SEQUENCE};
use crate::xsd::element::Element;
use crate::xsd::group::Group;
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::sequence::Sequence;

#[derive(Clone, Debug)]
pub struct Choice {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub choices: Choices,
}

#[derive(Clone, Debug)]
pub enum ChoiceItem {
    Element(Element),
    Group(Group),
    Sequence(Sequence),
}

pub type Choices = Vec<ChoiceItem>;

impl Choice {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage) -> Result<Self> {
        if node.name.as_str() != CHOICE {
            return raise!("expected '{}', got '{}'", CHOICE, node.name.as_str());
        }
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut choices = Choices::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone())?),
                ELEMENT => choices.push(ChoiceItem::Element(Element::from_xml(
                    inner,
                    lineage.clone(),
                )?)),
                GROUP => choices.push(ChoiceItem::Group(Group::from_xml(inner, lineage.clone())?)),
                SEQUENCE => choices.push(ChoiceItem::Sequence(Sequence::from_xml(
                    inner,
                    lineage.clone(),
                )?)),
                _ => return raise!("cannot parse '{}', unexpected node '{}'", CHOICE, t),
            }
        }
        Ok(Choice {
            id,
            annotation,
            choices,
        })
    }
}
/*
   <xs:choice>
       <xs:element name="credit-image" type="image"/>
       <xs:sequence>
           <xs:element name="credit-words" type="formatted-text"/>
           <xs:sequence minOccurs="0" maxOccurs="unbounded">
               <xs:element name="link" type="link" minOccurs="0" maxOccurs="unbounded"/>
               <xs:element name="bookmark" type="bookmark" minOccurs="0" maxOccurs="unbounded"/>
               <xs:element name="credit-words" type="formatted-text"/>
           </xs:sequence>
       </xs:sequence>
   </xs:choice>
*/

/*
<xs:sequence>
    <xs:choice>
        <xs:group ref="traditional-key"/>
        <xs:group ref="non-traditional-key" minOccurs="0" maxOccurs="unbounded"/>
    </xs:choice>
    <xs:element name="key-octave" type="key-octave" minOccurs="0" maxOccurs="unbounded">
        <xs:annotation>
            <xs:documentation>The optional list of key-octave elements is used to specify in which octave each element of the key signature appears.</xs:documentation>
        </xs:annotation>
    </xs:element>
</xs:sequence>
*/
#[test]
fn parse_credit() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let xml_str = r#"
    <xs:choice>
        <xs:element name="credit-image" type="image"/>
        <xs:sequence>
            <xs:element name="credit-words" type="formatted-text"/>
            <xs:sequence minOccurs="0" maxOccurs="unbounded">
                <xs:element name="link" type="link" minOccurs="0" maxOccurs="unbounded"/>
                <xs:element name="bookmark" type="bookmark" minOccurs="0" maxOccurs="unbounded"/>
                <xs:element name="credit-words" type="formatted-text"/>
            </xs:sequence>
        </xs:sequence>
    </xs:choice>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let ch = Choice::from_xml(&xml, Lineage::Parent(parent)).unwrap();
    assert_eq!(
        format!("{}", ch.id),
        "element:foo:choice:863778347360799337"
    );
    assert_eq!(ch.documentation().as_str(), "");
    assert_eq!(ch.choices.len(), 2);
    let ele = if let ChoiceItem::Element(el) = ch.choices.get(0).unwrap() {
        el
    } else {
        panic!("expected Element");
    };
    assert_eq!(ele.name(), "credit-image");
    let seq = if let ChoiceItem::Sequence(s) = ch.choices.get(1).unwrap() {
        s
    } else {
        panic!("expected Sequence");
    };
    assert_eq!(seq.occurs.min_occurs, 1);
    assert_eq!(seq.occurs.max_occurs, Some(1));
}
