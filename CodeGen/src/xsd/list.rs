use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, LIST, NAME};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::Xsd;

#[derive(Clone, Debug)]
pub struct List {
    pub id: Id,
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(LIST, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
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
                annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?);
                break;
            }
        }
        Ok(List { id, annotation, item_type })
    }
}

#[test]
fn parse() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let lineage = Lineage::Parent(parent);
    let xml_str = r#"
    <xs:list itemType="xs:integer">
        <xs:annotation>
            <xs:documentation>Hello</xs:documentation>
        </xs:annotation>
    </xs:list>
        "#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_id = "element:foo:list:4941444217670409626".to_owned();
    let want_doc = "Hello";
    let list = List::from_xml(&xml, lineage, &Xsd::new("xs")).unwrap();
    let got_doc = list.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", list.id);
    assert_eq!(got_id, want_id);
    let want_item_type = "xs:integer".to_owned();
    assert_eq!(list.item_type, want_item_type);
}
