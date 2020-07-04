use crate::error::Result;
use crate::xsd::constants::{ANNOTATION, APP_INFO, DOCUMENTATION};
use crate::xsd::id::{EntryType, Id};
use std::fmt::{Debug, Formatter};

#[derive(Clone, Debug)]
pub struct Annotation {
    pub id: Id,
    pub index: u64,
    pub items: Vec<Item>,
}

#[derive(Clone, Debug)]
pub enum Item {
    Documentation(String),
    AppInfo(String),
}

impl Annotation {
    pub fn documentation(&self) -> String {
        for item in &self.items {
            if let Item::Documentation(s) = &item {
                return s.clone();
            }
        }
        "".to_owned()
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let mut items = Vec::new();
        if node.name.as_str() != ANNOTATION {
            return raise!("expected '{}', got '{}'", ANNOTATION, &node.name);
        }
        for inner in node.children() {
            let t = inner.name.as_str();
            if let Some(s) = inner.text() {
                match t {
                    DOCUMENTATION => items.push(Item::Documentation(s)),
                    APP_INFO => items.push(Item::AppInfo(s)),
                    _ => {
                        return raise!(
                            "expected either '{}' or '{}', got '{}'",
                            DOCUMENTATION,
                            APP_INFO,
                            t
                        );
                    }
                }
            }
        }
        let id = Id::new(EntryType::Annotation, format!("{}", index));
        Ok(Annotation { id, index, items })
    }
}

#[test]
fn parse() {
    let want_index: u64 = 13;
    let want_name = "13";
    let want_doc = "bishop is sleeping.";
    let xml = exile::Element {
        namespace: Some("xs".to_owned()),
        name: ANNOTATION.to_owned(),
        attributes: Default::default(),
        nodes: vec![exile::Node::Element(exile::Element {
            namespace: Some("xs".to_owned()),
            name: DOCUMENTATION.to_owned(),
            attributes: Default::default(),
            nodes: vec![exile::Node::Text(want_doc.to_owned())],
        })],
    };

    let annotation = Annotation::from_xml(&xml, want_index).unwrap();
    let got_doc = annotation.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = annotation.index;
    assert_eq!(got_index, want_index);
    let got_name = annotation.id.name().unwrap();
    assert_eq!(got_name, want_name);
    // let got_type = annotation.id.entry_type;
    // assert_eq!(got_type, EntryType::Annotation);
}
