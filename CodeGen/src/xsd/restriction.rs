use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::simple_type::LIST;
use crate::xsd::{EntryType, ANNOTATION, ID};
use std::convert::TryInto;

/*
enumeration	Defines a list of acceptable values
fractionDigits	Specifies the maximum number of decimal places allowed. Must be equal to or greater than zero
length	Specifies the exact number of characters or list items allowed. Must be equal to or greater than zero
maxExclusive	Specifies the upper bounds for numeric values (the value must be less than this value)
maxInclusive	Specifies the upper bounds for numeric values (the value must be less than or equal to this value)
maxLength	Specifies the maximum number of characters or list items allowed. Must be equal to or greater than zero
minExclusive	Specifies the lower bounds for numeric values (the value must be greater than this value)
minInclusive	Specifies the lower bounds for numeric values (the value must be greater than or equal to this value)
minLength	Specifies the minimum number of characters or list items allowed. Must be equal to or greater than zero
pattern	Defines the exact sequence of characters that are acceptable
totalDigits	Specifies the exact number of digits allowed. Must be greater than zero
whiteSpace	Specifies how white space (line feeds, tabs, spaces, and carriage returns) is handled
 */

/*
<restriction
id=ID
base=QName
any attributes
>

Content for simpleType:
(annotation?,(simpleType?,(minExclusive|minInclusive|
maxExclusive|maxInclusive|totalDigits|fractionDigits|
length|minLength|maxLength|enumeration|whiteSpace|pattern)*))
 */

/*
<restriction
  base = QName
  id = ID
  {any attributes with non-schema namespace . . .}>
  Content: (annotation?, (simpleType?, (minExclusive | minInclusive | maxExclusive | maxInclusive |
  totalDigits | fractionDigits | length | minLength | maxLength | enumeration | whiteSpace |
  pattern | assertion | explicitTimezone | {any with namespace: ##other})*))
</restriction>
 */
// TODO - support additional facets
const ENUMERATION: &str = "enumeration";
const LENGTH: &str = "length";
const MAX_EXCLUSIVE: &str = "maxExclusive";
const MAX_INCLUSIVE: &str = "maxInclusive";
const MAX_LENGTH: &str = "maxLength";
const MIN_EXCLUSIVE: &str = "minExclusive";
const MIN_INCLUSIVE: &str = "minInclusive";
const MIN_LENGTH: &str = "minLength";
const PATTERN: &str = "pattern";

pub enum RestrictionType {
    Enumeration,
    Length,
    MaxExclusive,
    MaxInclusive,
    MaxLength,
    MinExclusive,
    MinInclusive,
    MinLength,
    Pattern,
}

pub enum RestrictionValue {
    Enumeration(String),
    Length(u64),
    MaxExclusive,
    MaxInclusive,
    MaxLength(u64),
    MinExclusive,
    MinInclusive,
    MinLength(u64),
    Pattern(String),
}

pub struct Restriction {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    // TODO
    //pub item_type: String,
}

impl Restriction {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != LIST {
            return raise!("expected '{}', got '{}'", LIST, &node.name);
        }
        let item_type = node
            .attributes
            .map()
            .get("ITEM_TYPE")
            .ok_or_else(|| make_err!("'{}' attribute not found", "ITEM_TYPE"))?
            .clone();
        let mut annotation = None;
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
                break;
            }
        }
        // TODO - this may not be unique
        let id = ID {
            entry_type: EntryType::Other(LIST.to_owned()),
            name: format!("{}", index),
        };
        Ok(Restriction {
            id,
            index,
            annotation,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
    <xs:list itemType="xs:integer">
        <xs:annotation>
            <xs:documentation>Hello</xs:documentation>
        </xs:annotation>
        "#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 5;
    let want_id = "5 (list)".to_owned();
    let want_doc = "Hello";
    let list = Restriction::from_xml(&xml, want_index).unwrap();
    let got_doc = list.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = list.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", list.id);
    assert_eq!(got_id, want_id);
    let got_type = list.id.entry_type;
    assert_eq!(got_type, EntryType::Other(LIST.to_owned()));
    // let want_item_type = "xs:integer".to_owned();
    // assert_eq!(list.item_type, want_item_type);
}
