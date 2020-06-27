use crate::error::{Error, Result};
use crate::xsd;
use crate::xsd::annotation::Annotation;
use crate::xsd::annotation::Item::Documentation;
use crate::xsd::restriction::FacetType::Pattern;
use crate::xsd::simple_type::{LIST, RESTRICTION};
use crate::xsd::{EntryType, ANNOTATION, ID};
use exile::Element;
use std::convert::TryInto;
use std::fmt::{Display, Formatter};
use std::str::FromStr;

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

/// Attributes
const BASE: &str = "base";
const VALUE: &str = "value";

#[derive(Debug, Clone, Copy, Eq, PartialEq, Ord, PartialOrd)]
pub enum FacetType {
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

impl Display for FacetType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            FacetType::Enumeration => ENUMERATION,
            FacetType::Length => LENGTH,
            FacetType::MaxExclusive => MAX_EXCLUSIVE,
            FacetType::MaxInclusive => MAX_INCLUSIVE,
            FacetType::MaxLength => MAX_LENGTH,
            FacetType::MinExclusive => MIN_EXCLUSIVE,
            FacetType::MinInclusive => MIN_INCLUSIVE,
            FacetType::MinLength => MIN_LENGTH,
            FacetType::Pattern => PATTERN,
        };
        write!(f, "{}", s)
    }
}

impl FacetType {
    fn parse<S: AsRef<str>>(s: S) -> Result<FacetType> {
        let s = s.as_ref();
        match s {
            ENUMERATION => Ok(FacetType::Enumeration),
            LENGTH => Ok(FacetType::Length),
            MAX_EXCLUSIVE => Ok(FacetType::MaxExclusive),
            MAX_INCLUSIVE => Ok(FacetType::MaxInclusive),
            MAX_LENGTH => Ok(FacetType::MaxLength),
            MIN_EXCLUSIVE => Ok(FacetType::MinExclusive),
            MIN_INCLUSIVE => Ok(FacetType::MinInclusive),
            MIN_LENGTH => Ok(FacetType::MinLength),
            PATTERN => Ok(FacetType::Pattern),
            _ => raise!("unsupported or invalid facet type {}", s),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Number {
    Integer(i64),
    Decimal(f64),
}

impl Number {
    fn parse<S: AsRef<str>>(s: S) -> Result<Number> {
        if let Ok(i) = s.as_ref().parse::<i64>() {
            return Ok(Number::Integer(i));
        }
        Ok(Number::Decimal(wrap!(s.as_ref().parse::<f64>())?))
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Facet {
    Enumeration(String),
    Length(u64),
    MaxExclusive(Number),
    MaxInclusive(Number),
    MaxLength(u64),
    MinExclusive(Number),
    MinInclusive(Number),
    MinLength(u64),
    Pattern(String),
}

impl Facet {
    fn from_xml(node: &Element) -> Result<Facet> {
        let t = FacetType::parse(&node.name)?;
        let v = node
            .attributes
            .map()
            .get(VALUE)
            .ok_or(make_err!("attribute '{}' not found", VALUE))?;
        let result = match t {
            FacetType::Enumeration => Facet::Enumeration(v.clone()),
            FacetType::Length => {
                let x = wrap!(v.parse::<u64>())?;
                Facet::Length(x)
            }
            FacetType::MaxExclusive => {
                let n = Number::parse(v)?;
                Facet::MaxExclusive(n)
            }
            FacetType::MaxInclusive => {
                let n = Number::parse(v)?;
                Facet::MaxInclusive(n)
            }
            FacetType::MaxLength => {
                let x = wrap!(v.parse::<u64>())?;
                Facet::MaxLength(x)
            }
            FacetType::MinExclusive => {
                let n = Number::parse(v)?;
                Facet::MinExclusive(n)
            }
            FacetType::MinInclusive => {
                let n = Number::parse(v)?;
                Facet::MinInclusive(n)
            }
            FacetType::MinLength => {
                let x = wrap!(v.parse::<u64>())?;
                Facet::MinLength(x)
            }
            Pattern => Facet::Pattern(v.clone()),
        };
        Ok(result)
    }
}

pub struct Restriction {
    pub id: ID,
    pub index: u64,
    pub annotation: Option<Annotation>,
    pub base: String,
    pub facets: Vec<Facet>,
}

impl Restriction {
    pub fn documentation(&self) -> String {
        if let Some(annotation) = &self.annotation {
            return annotation.documentation();
        }
        return "".to_owned();
    }

    pub fn from_xml(node: &exile::Element, index: u64) -> Result<Self> {
        if node.name.as_str() != RESTRICTION {
            return raise!("expected '{}', got '{}'", RESTRICTION, &node.name);
        }
        let base = node
            .attributes
            .map()
            .get(BASE)
            .ok_or_else(|| make_err!("'{}' attribute not found", BASE))?
            .clone();
        let mut annotation = None;
        let mut facets = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, index)?);
            } else {
                let facet = Facet::from_xml(inner)?;
                facets.push(facet);
            }
        }
        // TODO - this may not be unique
        let id = ID {
            entry_type: EntryType::Other(RESTRICTION.to_owned()),
            name: format!("{}", index),
        };
        Ok(Restriction {
            id,
            index,
            annotation,
            base,
            facets,
        })
    }
}

#[test]
fn parse() {
    let xml_str = r#"
		<xs:restriction base="xs:positiveInteger">
			<xs:minInclusive value="1"/>
			<xs:maxInclusive value="8"/>
		</xs:restriction>
        "#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_index: u64 = 3;
    let want_id = "3 (restriction)".to_owned();
    let want_doc = "";
    let r = Restriction::from_xml(&xml, want_index).unwrap();
    let got_doc = r.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_index = r.index;
    assert_eq!(got_index, want_index);
    let got_id = format!("{}", r.id);
    assert_eq!(got_id, want_id);
    let got_type = r.id.entry_type;
    assert_eq!(got_type, EntryType::Other(RESTRICTION.to_owned()));
    let got_base = r.base.as_str();
    let want_base = "xs:positiveInteger";
    assert_eq!(got_base, want_base);
    assert_eq!(r.facets.len(), 2);
    let min = r.facets.get(0).unwrap();
    if let Facet::MinInclusive(n) = min {
        if let Number::Integer(i) = n {
            assert_eq!(*i, 1);
        } else {
            panic!("wrong number type");
        }
    } else {
        panic!("wrong facet type");
    }
    let max = r.facets.get(1).unwrap();
    if let Facet::MaxInclusive(n) = max {
        if let Number::Integer(i) = n {
            assert_eq!(*i, 8);
        } else {
            panic!("wrong number type");
        }
    } else {
        panic!("wrong facet type");
    }
    // let want_item_type = "xs:integer".to_owned();
    // assert_eq!(list.item_type, want_item_type);
}
