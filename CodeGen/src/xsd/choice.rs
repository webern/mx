use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, CHOICE, ELEMENT, GROUP, NAME, SEQUENCE};
use crate::xsd::element::Element;
use crate::xsd::group::Group;
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::sequence::Sequence;
use crate::xsd::Xsd;
use std::cmp::Ordering;
use std::fmt::{Debug, Formatter};

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Choice {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub choices: Choices,
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub enum ChoiceItem {
    Element(Element),
    Group(Group),
    Sequence(Sequence),
    Choice(Box<Choice>),
}

pub struct Choices(Vec<ChoiceItem>);

impl Choice {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(CHOICE, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut choices = Choices::new();
        for inner in node.children() {
            let t = inner.name();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                ELEMENT => choices.push(ChoiceItem::Element(Element::from_xml(
                    inner,
                    lineage.clone(),
                    xsd,
                )?)),
                GROUP => choices.push(ChoiceItem::Group(Group::from_xml(
                    inner,
                    lineage.clone(),
                    xsd,
                )?)),
                SEQUENCE => choices.push(ChoiceItem::Sequence(Sequence::from_xml(
                    inner,
                    lineage.clone(),
                    xsd,
                )?)),
                CHOICE => {
                    let inner_choice = Choice::from_xml(inner, lineage.clone(), xsd)?;
                    let inner_box = Box::new(inner_choice);
                    let item = ChoiceItem::Choice(inner_box);
                    choices.push(item);
                }
                _ => return raise!("cannot parse {} '{}', unexpected node '{}'", CHOICE, id, t),
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
    let ch = Choice::from_xml(&xml, Lineage::Parent(parent), &Xsd::new("xs")).unwrap();
    assert_eq!(
        format!("{}", ch.id),
        "element:foo:choice:863778347360799337"
    );
    assert_eq!(ch.documentation().as_str(), "");
    assert_eq!(ch.choices.inner().len(), 2);
    let ele = if let ChoiceItem::Element(el) = ch.choices.inner().get(0).unwrap() {
        el
    } else {
        panic!("expected Element");
    };
    assert_eq!(ele.name(), "credit-image");
    let seq = if let ChoiceItem::Sequence(s) = ch.choices.inner().get(1).unwrap() {
        s
    } else {
        panic!("expected Sequence");
    };
    assert_eq!(seq.occurs.min_occurs, 1);
    assert_eq!(seq.occurs.max_occurs, Some(1));
}

// What a pain in the ass!
// #[derive(Clone, Debug, PartialOrd, PartialEq)]
impl Choices {
    pub fn new() -> Self {
        Self(Vec::new())
    }

    pub fn push(&mut self, item: ChoiceItem) {
        self.0.push(item)
    }

    pub fn inner(&self) -> &Vec<ChoiceItem> {
        &self.0
    }

    pub fn mut_inner(&mut self) -> &mut Vec<ChoiceItem> {
        &mut self.0
    }
}

impl Clone for Choices {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

impl Debug for Choices {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        Debug::fmt(&self.0, f)
    }
}

impl PartialEq for Choices {
    fn eq(&self, other: &Self) -> bool {
        self.0.eq(&other.0)
    }
}

impl PartialOrd for Choices {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.0.partial_cmp(&other.0)
    }
}
