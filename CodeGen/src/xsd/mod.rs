use core::fmt;
use std::fmt::{Debug, Display, Formatter};
use std::panic::resume_unwind;

// use derive_more::Display;

use crate::error::{self, Result};

pub(crate) const XS_STRING: &str = "xs:string";
pub(crate) const XS_TOKEN: &str = "xs:token";
pub(crate) const XS_DECIMAL: &str = "xs:decimal";
pub(crate) const XS_INTEGER: &str = "xs:integer";
pub(crate) const XS_NON_NEGATIVE_INTEGER: &str = "xs:nonNegativeInteger";
pub(crate) const XS_POSITIVE_INTEGER: &str = "xs:positiveInteger";
pub(crate) const XS_DATE: &str = "xs:date";

pub(crate) type UInt = u64;
pub(crate) type Int = i64;

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(crate) enum BuiltinStringType {
    XsString,
    XsToken,
}

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(crate) enum BuiltinNumericType {
    XsDecimal,
    XsInteger,
    XsNonNegativeInteger,
    XsPositiveInteger,
}

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(crate) enum BuiltinType {
    Numeric(BuiltinNumericType),
    String(BuiltinStringType),
    XsDate,
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(crate) enum Base {
    BuiltinType(BuiltinType),
    CustomType(String),
}

impl Base {
    pub(crate) fn parse<S: AsRef<str>>(s: S) -> Result<Self> {
        let s = s.as_ref();
        if !s.starts_with("xs:") {
            Ok(Base::CustomType(s.into()))
        } else {
            match s {
                XS_STRING => Ok(Base::BuiltinType(BuiltinType::String(
                    BuiltinStringType::XsString,
                ))),
                XS_TOKEN => Ok(Base::BuiltinType(BuiltinType::String(
                    BuiltinStringType::XsToken,
                ))),
                XS_DECIMAL => Ok(Base::BuiltinType(BuiltinType::Numeric(
                    BuiltinNumericType::XsDecimal,
                ))),
                XS_INTEGER => Ok(Base::BuiltinType(BuiltinType::Numeric(
                    BuiltinNumericType::XsInteger,
                ))),
                XS_NON_NEGATIVE_INTEGER => Ok(Base::BuiltinType(BuiltinType::Numeric(
                    BuiltinNumericType::XsNonNegativeInteger,
                ))),
                XS_POSITIVE_INTEGER => Ok(Base::BuiltinType(BuiltinType::Numeric(
                    BuiltinNumericType::XsPositiveInteger,
                ))),
                XS_DATE => Ok(Base::BuiltinType(BuiltinType::XsDate)),
                _ => raise!("unsupported base: '{}'", s),
            }
        }
    }
}

impl Display for BuiltinStringType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            BuiltinStringType::XsString => write!(f, "{}", XS_STRING),
            BuiltinStringType::XsToken => write!(f, "{}", XS_TOKEN),
        }
    }
}

impl Debug for BuiltinStringType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        Display::fmt(self, f)
    }
}

impl Display for BuiltinNumericType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            BuiltinNumericType::XsDecimal => write!(f, "{}", XS_DECIMAL),
            BuiltinNumericType::XsInteger => write!(f, "{}", XS_INTEGER),
            BuiltinNumericType::XsNonNegativeInteger => write!(f, "{}", XS_NON_NEGATIVE_INTEGER),
            BuiltinNumericType::XsPositiveInteger => write!(f, "{}", XS_POSITIVE_INTEGER),
        }
    }
}

impl Debug for BuiltinType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        Display::fmt(self, f)
    }
}

impl Display for BuiltinType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            BuiltinType::Numeric(x) => Display::fmt(&x, f),
            BuiltinType::String(x) => Display::fmt(&x, f),
            BuiltinType::XsDate => write!(f, "{}", XS_DATE),
        }
    }
}

impl Debug for Base {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        Display::fmt(self, f)
    }
}

impl Display for Base {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            Base::BuiltinType(x) => Display::fmt(&x, f),
            Base::CustomType(s) => Display::fmt(&s, f),
        }
    }
}

impl Default for Base {
    fn default() -> Self {
        Base::BuiltinType(BuiltinType::Numeric(BuiltinNumericType::XsPositiveInteger))
    }
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
pub(crate) enum SimpleDerivation {
    Restriction(Restriction),
    Union(Vec<Base>),
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug, Default)]
pub(crate) struct Restriction {
    pub(crate) base: Base,
    pub(crate) enumerations: Vec<String>,
    pub(crate) patterns: Vec<String>,
    // not used in musicxml
    // min_length: Option<UInt>,
    // max_length: Option<UInt>,
}

impl Restriction {
    pub(crate) fn parse(x: &exile::Element) -> Result<SimpleDerivation> {
        let mut restriction = Restriction::default();
        let base_str = x
            .attributes
            .map()
            .get("base")
            .ok_or(make_err!("base attribute not found"))?;
        let base = Base::parse(base_str)?;
        restriction.base = base;
        for child in x.children() {
            if child.name == "enumeration" {
                let value = child
                    .attributes
                    .map()
                    .get("value")
                    .ok_or(make_err!("value attribute not found"))?;
                restriction.enumerations.push(value.into());
            } else if child.name == "pattern" {
                let value = child
                    .attributes
                    .map()
                    .get("value")
                    .ok_or(make_err!("value attribute not found"))?;
                restriction.patterns.push(value.into());
            }
        }
        Ok(SimpleDerivation::Restriction(restriction))
    }
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
pub(crate) struct SimpleType {
    pub(crate) name: String,
    pub(crate) derivation: SimpleDerivation,
}

impl SimpleType {
    pub(crate) fn new(name: String) -> Self {
        SimpleType {
            name,
            derivation: SimpleDerivation::Restriction(Restriction::default()),
        }
    }

    pub(crate) fn parse(x: &exile::Element) -> Result<Self> {
        let name = x
            .attributes
            .map()
            .get("name")
            .ok_or(make_err!("name attribute not found"))?;
        let mut simple_type = SimpleType::new(name.into());
        if let Some(r) = x.child("restriction") {
            simple_type.derivation = Restriction::parse(r)?;
        } else if let Some(u) = x.child("union") {
            if let Some(memberTypes) = u.attributes.map().get("memberTypes") {
                // TODO parse the string
            }
            // TODO - parse child element members
            simple_type.derivation = SimpleDerivation::Union(Vec::new());
        }

        Ok(simple_type)
    }
}
