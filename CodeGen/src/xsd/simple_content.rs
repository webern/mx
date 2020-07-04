use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, EXTENSION, NAME, SIMPLE_CONTENT};
use crate::xsd::extension::Extension;
use crate::xsd::id::{Id, Lineage, RootNodeType};

#[derive(Clone, Debug)]
pub struct SimpleContent {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

#[derive(Clone, Debug)]
pub enum Payload {
    Extension(Extension),
}

impl SimpleContent {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage) -> Result<Self> {
        if node.name.as_str() != SIMPLE_CONTENT {
            return raise!("expected '{}', got '{}'", SIMPLE_CONTENT, &node.name);
        }
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone())?),
                EXTENSION => {
                    payload = Some(Payload::Extension(Extension::from_xml(
                        inner,
                        lineage.clone(),
                    )?));
                }
                _ => return raise!("unsupported simpleContent node '{}'", t),
            }
        }
        let payload = if let Some(p) = payload {
            p
        } else {
            return raise!("{} payload was not found", SIMPLE_CONTENT);
        };
        Ok(SimpleContent {
            id,
            annotation,
            payload,
        })
    }
}

#[test]
fn parse() {
    use super::attribute_group::AttributeGroup;
    use super::attributes::AttributeItem;
    let parent = crate::xsd::id::Id::sldkjfgh(crate::xsd::id::RootNodeType::Element, "foo".into());
    let lineage = Lineage::Parent(parent);
    let xml_str = r#"
    <xs:simpleContent>
		<xs:extension base="xs:string">
			<xs:attributeGroup ref="part-name-text"/>
		</xs:extension>
	</xs:simpleContent>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_id = "element:foo:simpleContent:17852308769138094955".to_owned();
    let want_doc = "";
    let sc = SimpleContent::from_xml(&xml, lineage).unwrap();
    let got_doc = sc.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", sc.id);
    assert_eq!(got_id, want_id);
    // let got_type = sc.id.entry_type;
    // assert_eq!(got_type, RootNodeType::Other(SIMPLE_CONTENT.to_owned()));
    match sc.payload {
        Payload::Extension(ext) => {
            assert_eq!(ext.attributes.len(), 1);
            let a = ext.attributes.get(0).unwrap();
            match a {
                AttributeItem::Attribute(_) => {
                    panic!("expected 'AttributeGroup' but got 'Attribute'")
                }
                AttributeItem::AttributeGroup(x) => match x {
                    AttributeGroup::Def(_) => panic!("expected Ref got Def"),
                    AttributeGroup::Ref(r) => assert_eq!(r.ref_.as_str(), "part-name-text"),
                },
            }
        }
    }
}
