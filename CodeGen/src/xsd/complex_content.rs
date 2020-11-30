use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{ANNOTATION, COMPLEX_CONTENT, EXTENSION, NAME};
use crate::xsd::extension::Extension;
use crate::xsd::id::{Id, Lineage, RootNodeType};
use crate::xsd::Xsd;

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ComplexContent {
    pub id: Id,
    pub annotation: Option<Annotation>,
    pub extension: Extension,
}

impl ComplexContent {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, lineage: Lineage, xsd: &Xsd) -> Result<Self> {
        check!(COMPLEX_CONTENT, node, xsd)?;
        let (id, lineage) = Id::make(lineage, node)?;
        let mut annotation = None;
        let mut extension = None;
        for inner in node.children() {
            let t = inner.name();
            match t {
                ANNOTATION => annotation = Some(Annotation::from_xml(inner, lineage.clone(), xsd)?),
                EXTENSION => {
                    extension = Some(Extension::from_xml(inner, lineage.clone(), xsd)?);
                }
                _ => return raise!("unsupported simpleContent node '{}'", t),
            }
        }
        let extension = if let Some(ext) = extension {
            ext
        } else {
            return raise!("{} payload was not found", COMPLEX_CONTENT);
        };
        Ok(ComplexContent {
            id,
            annotation,
            extension,
        })
    }
}

#[test]
fn parse() {
    use super::attributes::AttributeItem;
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let lineage = Lineage::Parent(parent);
    let xml_str = r#"
		<xs:complexContent>
			<xs:extension base="time-modification">
				<xs:attribute name="type" type="start-stop" use="required"/>
				<xs:attribute name="bracket" type="yes-no"/>
				<xs:attribute name="show-number" type="show-tuplet"/>
			</xs:extension>
		</xs:complexContent>"#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_id = "element:foo:complexContent:10793997753492324063".to_owned();
    let want_doc = "";
    let sc = ComplexContent::from_xml(&xml, lineage, &Xsd::new("xs")).unwrap();
    let got_doc = sc.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", sc.id);
    assert_eq!(got_id, want_id);
    // let got_type = sc.id.entry_type;
    // assert_eq!(got_type, RootNodeType::Other(COMPLEX_CONTENT.to_owned()));
    let extension = &sc.extension;
    assert_eq!(extension.base.as_str(), "time-modification");
    assert_eq!(extension.attributes.len(), 3);
    let a = extension.attributes.get(0).unwrap();
    match a {
        AttributeItem::Attribute(x) => {
            assert_eq!(x.name.as_str(), "type");
            assert!(x.defined_by.is_type());
            assert_eq!(x.defined_by.value(), "start-stop");
            assert!(x.required);
        }
        AttributeItem::AttributeGroup(_) => panic!("expected 'Attribute' but got 'AttributeGroup'"),
    }
}
