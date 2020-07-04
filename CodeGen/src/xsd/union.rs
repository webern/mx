use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, MEMBER_TYPES, UNION};
use crate::xsd::id::{Id, RootNodeType};

#[derive(Clone, Debug)]
pub struct Union {
    pub id: Id,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub members: Vec<String>,
}

impl Union {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        let mut items = Vec::new();
        if node.name.as_str() != UNION {
            return raise!("expected '{}', got '{}'", UNION, &node.name);
        }
        let members = node
            .attributes
            .map()
            .get(MEMBER_TYPES)
            .ok_or_else(|| make_err!("'{}' attribute not found", MEMBER_TYPES))?;
        for item in members.split(' ') {
            if !item.is_empty() {
                items.push(item.to_owned());
            }
        }
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        let id = Id::new(RootNodeType::Other(UNION.to_owned()), format!("{}", index));
        Ok(Union {
            id,
            index,
            annotation,
            members: items,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"<xs:union memberTypes="xs:decimal css-font-size"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 0;
    let want_id = "union:0".to_owned();
    let want_doc = "";
    let union = Union::from_xml(&xml, want_index).unwrap();
    let got_doc = union.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = union.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", union.id);
    assert_eq!(got_id, want_id);
    // let got_type = union.id.entry_type;
    // assert_eq!(got_type, EntryType::Other(UNION.to_owned()));
    let want_members = vec!["xs:decimal".to_owned(), "css-font-size".to_owned()];
    assert_eq!(union.members, want_members);
}
