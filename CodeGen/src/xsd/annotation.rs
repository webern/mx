use crate::error::{Error, Result};
use crate::xsd;
use std::convert::TryInto;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::ANNOTATION;

pub struct Annotation {
    pub items: Vec<Item>,
}

pub const DOCUMENTATION: &str = "documentation";
pub const APP_INFO: &str = "appinfo";

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

    pub fn from_xml(node: &exile::Element) -> Result<Self> {
        let mut items = Vec::new();
        if node.name.as_str() != xsd::ANNOTATION {
            return raise!("expected '{}', got '{}'", xsd::ANNOTATION, &node.name);
        }
        for inner in node.children() {
            let t = inner.name.as_str();
            if let Some(s) = inner.text() {
                match t {
                    DOCUMENTATION => { items.push(Item::Documentation(s)) }
                    APP_INFO => { items.push(Item::AppInfo(s)) }
                    _ => return raise!("expected either '{}' or '{}', got '{}'", DOCUMENTATION, APP_INFO, t)
                }
            }
        }
        Ok(Annotation { items })
    }
}

#[test]
fn parse() {
    let want = "bishop is sleeping.";
    let xml = exile::Element {
        namespace: Some("xs".to_owned()),
        name: ANNOTATION.to_owned(),
        attributes: Default::default(),
        nodes: vec![exile::Node::Element(exile::Element {
            namespace: Some("xs".to_owned()),
            name: DOCUMENTATION.to_owned(),
            attributes: Default::default(),
            nodes: vec![exile::Node::Text(want.to_owned())],
        })],
    };

    let annotation = Annotation::from_xml(&xml).unwrap();
    let got = annotation.documentation();
    assert_eq!(got.as_str(), want);
}