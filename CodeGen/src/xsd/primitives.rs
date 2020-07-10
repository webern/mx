use crate::error::Result;
use crate::model::builtin::Builtin::Number;
use crate::xsd::constants::*;
use std::fmt::{Display, Formatter};

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Numeric {
    Byte,
    Decimal,
    Int,
    Integer,
    Long,
    NegativeInteger,
    NonNegativeInteger,
    NonPositiveInteger,
    PositiveInteger,
    Short,
    UnsignedLong,
    UnsignedInt,
    UnsignedShort,
    UnsignedByte,
}

impl Numeric {
    /// `s` is the string to be parsed, e.g. `"integer"`, `prefix` is the namespace prefix in use,
    /// usually `"xs"` or `"xsd"`.
    pub fn parse<S1, S2>(s: S1, prefix: S2) -> Result<Self>
    where
        S1: AsRef<str>,
        S2: AsRef<str>,
    {
        let s = s.as_ref();
        let mut split = s.split(":");
        let mut ns: &str = split.next().unwrap_or("");
        let mut val: &str = split.next().unwrap_or("");
        if val.is_empty() && !ns.is_empty() {
            // if there no val but there is an ns, then it means that ns was actually the val.
            // e.g. "blah" -> ns="blah" val="", so we swap to ns="" val="blah"
            val = ns;
            ns = "";
        }
        if ns != prefix.as_ref() {
            return raise!(
                "wrong namespace prefix. expected '{}', got '{}'",
                prefix.as_ref(),
                ns
            );
        }
        match val {
            BYTE => Ok(Numeric::Byte),
            DECIMAL => Ok(Numeric::Decimal),
            INT => Ok(Numeric::Int),
            INTEGER => Ok(Numeric::Integer),
            LONG => Ok(Numeric::Long),
            NEGATIVE_INTEGER => Ok(Numeric::NegativeInteger),
            NON_NEGATIVE_INTEGER => Ok(Numeric::NonNegativeInteger),
            NON_POSITIVE_INTEGER => Ok(Numeric::NonPositiveInteger),
            POSITIVE_INTEGER => Ok(Numeric::PositiveInteger),
            SHORT => Ok(Numeric::Short),
            UNSIGNED_LONG => Ok(Numeric::UnsignedLong),
            UNSIGNED_INT => Ok(Numeric::UnsignedInt),
            UNSIGNED_SHORT => Ok(Numeric::UnsignedShort),
            UNSIGNED_BYTE => Ok(Numeric::UnsignedByte),
            _ => raise!("unknown numeric type: '{}'", s),
        }
    }

    pub fn str(&self) -> &'static str {
        match self {
            Numeric::Byte => BYTE,
            Numeric::Decimal => DECIMAL,
            Numeric::Int => INT,
            Numeric::Integer => INTEGER,
            Numeric::Long => LONG,
            Numeric::NegativeInteger => NEGATIVE_INTEGER,
            Numeric::NonNegativeInteger => NON_NEGATIVE_INTEGER,
            Numeric::NonPositiveInteger => NON_POSITIVE_INTEGER,
            Numeric::PositiveInteger => POSITIVE_INTEGER,
            Numeric::Short => SHORT,
            Numeric::UnsignedLong => UNSIGNED_LONG,
            Numeric::UnsignedInt => UNSIGNED_INT,
            Numeric::UnsignedShort => UNSIGNED_SHORT,
            Numeric::UnsignedByte => UNSIGNED_BYTE,
        }
    }
}

impl Display for Numeric {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.str())
    }
}
