use crate::error::Result;
use crate::xsd::annotation::Annotation;

use crate::xsd::constants::{ANNOTATION, CHOICE, ELEMENT, GROUP, NAME, SEQUENCE};
use crate::xsd::element::Element;
use crate::xsd::group::Group;
use crate::xsd::sequence::Sequence;
use crate::xsd::{EntryType, ID};

/*
element
group
sequence
 */

pub struct Choice {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub choices: Choices,
}

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

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != CHOICE {
            return raise!("expected '{}', got '{}'", CHOICE, node.name.as_str());
        }
        let mut annotation = None;
        let mut choices = Choices::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, index)?),
                ELEMENT => choices.push(ChoiceItem::Element(Element::from_xml(inner, index)?)),
                GROUP => choices.push(ChoiceItem::Group(Group::from_xml(inner, index)?)),
                SEQUENCE => choices.push(ChoiceItem::Sequence(Sequence::from_xml(inner, index)?)),
                _ => return raise!("cannot parse '{}', unexpected node '{}'", CHOICE, t),
            }
        }
        let id = ID {
            entry_type: EntryType::Other(CHOICE.into()),
            name: Self::name_or_index(node, index),
        };
        Ok(Choice {
            id,
            index,
            annotation,
            choices,
        })
    }

    fn name_or_index(node: &exile::Element, index: u64) -> String {
        if let Some(n) = node.attributes.map().get(NAME) {
            n.clone()
        } else {
            format!("{}", index)
        }
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
    use super::simple_content;
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
    let ch = Choice::from_xml(&xml, 6).unwrap();
    assert_eq!(format!("{}", ch.id), "6 (choice)");
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
