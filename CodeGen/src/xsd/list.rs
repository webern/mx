use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, LIST};
use crate::xsd::{EntryType, Id};

#[derive(Clone, Debug)]
pub struct List {
    pub id: Id,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub item_type: String,
}

const ITEM_TYPE: &str = "itemType";

impl List {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != LIST {
            return raise!("expected '{}', got '{}'", LIST, &node.name);
        }
        let item_type = node
            .attributes
            .map()
            .get(ITEM_TYPE)
            .ok_or_else(|| make_err!("'{}' attribute not found", ITEM_TYPE))?
            .clone();
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        // TODO - this may not be unique
        let id = Id {
            entry_type: EntryType::Other(LIST.to_owned()),
            name: format!("{}", index),
        };
        Ok(List {
            id,
            index,
            annotation,
            item_type,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
    <xs:list itemType="xs:integer">
        <xs:annotation>
            <xs:documentation>Hello</xs:documentation>
        </xs:annotation>
    </xs:list>
        "#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 5;
    let want_id = "list:5".to_owned();
    let want_doc = "Hello";
    let list = List::from_xml(&xml, want_index).unwrap();
    let got_doc = list.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = list.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", list.id);
    assert_eq!(got_id, want_id);
    let got_type = list.id.entry_type;
    assert_eq!(got_type, EntryType::Other(LIST.to_owned()));
    let want_item_type = "xs:integer".to_owned();
    assert_eq!(list.item_type, want_item_type);
}
