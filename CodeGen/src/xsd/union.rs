use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, MEMBER_TYPES, NAME, UNION};
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::primitives::{BaseType, PrefixedParse};
use crate::xsd::Xsd;

#[derive(Clone, Debug)]
pub struct Union {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub members: Vec<BaseType>,
}

impl Union {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(UNION, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut items = Vec::new();
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
                annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?);
                break;
            }
        }
        let mut members = Vec::new();
        for item in &items {
            let base_type = BaseType::parse_prefixed(item.as_str(), xsd.prefix.as_str())?;
            members.push(base_type);
        }
        let u = Union {
            id,
            annotation,
            members,
        };
        Ok(u)
    }
}

// TODO fix test
#[test]
fn parse() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let lineage = Lineage::Parent(parent);
    let xml_str = r#"<xs:union memberTypes="xs:decimal css-font-size"/>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_id = "element:foo:union:18365985102726890478".to_owned();
    let want_doc = "";
    let union = Union::from_xml(&xml, lineage, &Xsd::new("xs")).unwrap();
    let got_doc = union.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", union.id);
    assert_eq!(got_id, want_id);
    // let got_type = union.id.entry_type;
    // assert_eq!(got_type, RootNodeType::Other(UNION.to_owned()));
    let want_members = vec![
        BaseType::Decimal,
        BaseType::Custom("css-font-size".to_owned()),
    ];
    assert_eq!(union.members, want_members);
}
