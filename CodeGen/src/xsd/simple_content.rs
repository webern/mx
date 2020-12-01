use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, EXTENSION, NAME, SIMPLE_CONTENT};
use crate::xsd::extension::Extension;
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::Xsd;

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SimpleContent {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub payload: Payload,
}

#[derive(Clone, Debug, PartialEq, PartialOrd)]
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(SIMPLE_CONTENT, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut payload = None;
        for inner in node.children() {
            let t = inner.name();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                EXTENSION => {
                    payload = Some(Payload::Extension(Extension::from_xml(
                        inner,
                        lineage.clone(),
                        xsd,
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
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
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
    let sc = SimpleContent::from_xml(&xml, lineage, &Xsd::new("xs")).unwrap();
    let got_doc = sc.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", sc.id);
    assert_eq!(got_id, want_id);
    // let got_type = sc.id.entry_type;
    // assert_eq!(got_type, RootNodeType::Other(SIMPLE_CONTENT.to_owned()));
    match sc.payload {
        Payload::Extension(ext) => {
            assert_eq!(ext.attributes.inner().len(), 1);
            let a = ext.attributes.inner().get(0).unwrap();
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
