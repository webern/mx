use crate::error::Result;
use crate::xsd::constants::{ANNOTATION, APP_INFO, DOCUMENTATION, NAME};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use std::fmt::{Debug, Formatter};

#[derive(Clone, Debug)]
pub struct Annotation {
    pub id: Id,
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage) -> Result<Self> {
        let (id, lineage) = Id::make(lineage, node)?;
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
        Ok(Annotation { id, items })
    }
}

#[test]
fn parse() {
    let want_index: u64 = 13;
    let want_name = "4594507136952412519";
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

    let annotation = Annotation::from_xml(&xml, Lineage::Index(want_index)).unwrap();
    let got_doc = annotation.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = annotation.id.index().unwrap();
    assert_eq!(got_index, want_index);
    let got_name = annotation.id.name();
    assert_eq!(got_name, want_name);
    let got_type = annotation.id.root_node_type().unwrap();
    assert_eq!(got_type, RootNodeType::Annotation);
}
