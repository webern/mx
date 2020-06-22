// use crate::error::{Error, Result};
// use crate::xsd;
// use std::convert::TryInto;
// use crate::xsd::annotation::Item::Documentation;
// use crate::xsd::{ANNOTATION, ID, EntryType};
//
// pub struct SimpleType {
//     pub id: ID,
//     pub index: u64,
// }
//
// pub const RESTRICTION: &str = "restriction";
// pub const APP_INFO: &str = "appinfo";
//
// pub enum Item {
//     Documentation(String),
//     AppInfo(String),
// }
//
// impl SimpleType {
//     pub fn documentation(&self) -> String {
//         for item in &self.items {
//             if let Item::Documentation(s) = &item {
//                 return s.clone();
//             }
//         }
//         "".to_owned()
//     }
//
//     pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
//         let mut items = Vec::new();
//         if node.name.as_str() != xsd::ANNOTATION {
//             return raise!("expected '{}', got '{}'", xsd::ANNOTATION, &node.name);
//         }
//         for inner in node.children() {
//             let t = inner.name.as_str();
//             if let Some(s) = inner.text() {
//                 match t {
//                     DOCUMENTATION => { items.push(Item::Documentation(s)) }
//                     APP_INFO => { items.push(Item::AppInfo(s)) }
//                     _ => return raise!("expected either '{}' or '{}', got '{}'", DOCUMENTATION, APP_INFO, t)
//                 }
//             }
//         }
//         let id = ID {
//             entry_type: EntryType::Annotation,
//             name: format!("{}", index),
//         };
//         Ok(Annotation { id, index, items })
//     }
// }