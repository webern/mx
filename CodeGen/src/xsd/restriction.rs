use crate::error::Result;
use crate::xsd::annotation::Annotation;
use crate::xsd::constants::{
    ANNOTATION, ENUMERATION, LENGTH, MAX_EXCLUSIVE, MAX_INCLUSIVE, MAX_LENGTH, MIN_EXCLUSIVE,
    MIN_INCLUSIVE, MIN_LENGTH, NAME, PATTERN, RESTRICTION,
};
use crate::xsd::restriction::FacetType::Pattern;
use crate::xsd::{base_attribute, value_attribute};
use exile::Element;

use crate::xsd::id::{Id, Lineage, RootNodeType};
use std::fmt::{Display, Formatter};

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

#[derive(Debug, Clone, Copy, PartialEq)]
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
        // let (id, lineage) = Id::make(lineage, node)?;
        let t = FacetType::parse(&node.name)?;
        let v = value_attribute(node)?;
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

#[derive(Clone, Debug)]
pub struct Restriction {
    pub id: Id,
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

    pub fn from_xml(node: &exile::Element, lineage: Lineage) -> Result<Self> {
        if node.name.as_str() != RESTRICTION {
            return raise!("expected '{}', got '{}'", RESTRICTION, &node.name);
        }
        let (id, lineage) = Id::make(lineage, node)?;
        let base = base_attribute(node)?;
        let mut annotation = None;
        let mut facets = Vec::new();
        for inner in node.children() {
            let t = inner.name.as_str();
            if t == ANNOTATION {
                annotation = Some(Annotation::from_xml(inner, lineage.clone())?);
            } else {
                let facet = Facet::from_xml(inner)?;
                facets.push(facet);
            }
        }

        Ok(Restriction {
            id,
            annotation,
            base,
            facets,
        })
    }
}

#[test]
fn parse() {
    let parent = crate::xsd::id::Id::new(crate::xsd::id::RootNodeType::Element, "foo".into());
    let lineage = Lineage::Parent(parent);
    let xml_str = r#"
		<xs:restriction base="xs:positiveInteger">
			<xs:minInclusive value="1"/>
			<xs:maxInclusive value="8"/>
		</xs:restriction>
        "#;
    let doc = exile::parse(xml_str).unwrap();
    let xml = doc.root();
    let want_id = "element:foo:restriction:18375205485067440936".to_owned();
    let want_doc = "";
    let r = Restriction::from_xml(&xml, lineage).unwrap();
    let got_doc = r.documentation();
    assert_eq!(got_doc.as_str(), want_doc);
    let got_id = format!("{}", r.id);
    assert_eq!(got_id, want_id);
    // let got_type = r.id.entry_type;
    // assert_eq!(got_type, RootNodeType::Other(RESTRICTION.to_owned()));
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
}
