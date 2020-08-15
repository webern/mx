use crate::error::{Error, Result};
use crate::model::builtin::Builtin::Number;
use crate::xsd::constants::*;
use std::borrow::Cow;
use std::convert::TryFrom;
use std::fmt::{Display, Formatter};
use std::io::BufRead;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Traits

pub trait PrefixedParse {
    type ParsedThing;

    /// Given a parseable thing, `parseable` return the parsed representation. e.g.
    /// `parse_name("something")` might return a `Something` variant.
    fn parse<S: AsRef<str>>(parseable: S) -> Result<Self::ParsedThing>;

    /// Given a string `s`, of the format "xs:something", and the expected prefix, e.g. in this case
    /// "xs", then parse the string and return the `Something` representation. Returns an error if
    /// the string's prefix does not match `prefix`. e.g. `parse("xsd:something", "foo")` returns an
    /// error because "xsd" does not equal "foo".
    fn parse_prefixed<S1, S2>(s: S1, prefix: S2) -> Result<Self::ParsedThing>
    where
        S1: AsRef<str>,
        S2: AsRef<str>,
    {
        let (ns, val) = split_raw_str(s.as_ref());
        if ns != prefix.as_ref() {
            return raise!(
                "wrong namespace prefix. expected '{}', got '{}'",
                prefix.as_ref(),
                ns
            );
        }
        Self::parse(val)
    }
}

pub trait PrefixedString: Display {
    fn name(&self) -> &str;

    fn as_str(&self, prefix: &str) -> Cow<'_, str> {
        let s = self.name();
        if prefix.is_empty() {
            Cow::Borrowed(s)
        } else {
            Cow::Owned(format!("{}:{}", prefix, s))
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// BaseType

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum BaseType {
    // Numerics
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

    // Strings
    ID,
    IDREF,
    Language,
    Name,
    NmToken,
    NormalizedString,
    String,
    Token,

    // Dates
    Date,
    DateTime,
    Duration,
    GDay,
    GMonth,
    GMonthDay,
    GYear,
    GYearMonth,
    Time,

    // Custom
    Custom(String),
}

impl PrefixedParse for BaseType {
    type ParsedThing = BaseType;

    fn parse<S: AsRef<str>>(parseable: S) -> Result<Self::ParsedThing> {
        let s = parseable.as_ref();
        match s {
            BYTE => Ok(BaseType::Byte),
            DECIMAL => Ok(BaseType::Decimal),
            INT => Ok(BaseType::Int),
            INTEGER => Ok(BaseType::Integer),
            LONG => Ok(BaseType::Long),
            NEGATIVE_INTEGER => Ok(BaseType::NegativeInteger),
            NON_NEGATIVE_INTEGER => Ok(BaseType::NonNegativeInteger),
            NON_POSITIVE_INTEGER => Ok(BaseType::NonPositiveInteger),
            POSITIVE_INTEGER => Ok(BaseType::PositiveInteger),
            SHORT => Ok(BaseType::Short),
            UNSIGNED_LONG => Ok(BaseType::UnsignedLong),
            UNSIGNED_INT => Ok(BaseType::UnsignedInt),
            UNSIGNED_SHORT => Ok(BaseType::UnsignedShort),
            UNSIGNED_BYTE => Ok(BaseType::UnsignedByte),
            //
            ID => Ok(BaseType::ID),
            IDREF => Ok(BaseType::IDREF),
            LANGUAGE => Ok(BaseType::Language),
            NAME => Ok(BaseType::Name),
            NMTOKEN => Ok(BaseType::NmToken),
            NORMALIZED_STRING => Ok(BaseType::NormalizedString),
            STRING => Ok(BaseType::String),
            TOKEN => Ok(BaseType::Token),
            //
            DATE => Ok(BaseType::Date),
            DATETIME => Ok(BaseType::DateTime),
            DURATION => Ok(BaseType::Duration),
            G_DAY => Ok(BaseType::GDay),
            G_MONTH => Ok(BaseType::GMonth),
            G_MONTH_DAY => Ok(BaseType::GMonthDay),
            G_YEAR => Ok(BaseType::GYear),
            G_YEAR_MONTH => Ok(BaseType::GYearMonth),
            TIME => Ok(BaseType::Time),
            //
            custom => Ok(BaseType::Custom(custom.into())),
        }
    }

    fn parse_prefixed<S1, S2>(s: S1, prefix: S2) -> Result<Self::ParsedThing>
    where
        S1: AsRef<str>,
        S2: AsRef<str>,
    {
        let prefix = prefix.as_ref();
        let s = s.as_ref();
        let (ns, val) = split_raw_str(s);
        if ns != prefix {
            return Ok(BaseType::Custom(s.into()));
        }
        Ok(match Self::parse(val)? {
            BaseType::Custom(_) => BaseType::Custom(s.into()),
            anything_else => anything_else,
        })
    }
}

impl PrefixedString for BaseType {
    fn name(&self) -> &str {
        match self {
            // Numerics
            BaseType::Byte => BYTE,
            BaseType::Decimal => DECIMAL,
            BaseType::Int => INT,
            BaseType::Integer => INTEGER,
            BaseType::Long => LONG,
            BaseType::NegativeInteger => NEGATIVE_INTEGER,
            BaseType::NonNegativeInteger => NON_NEGATIVE_INTEGER,
            BaseType::NonPositiveInteger => NON_POSITIVE_INTEGER,
            BaseType::PositiveInteger => POSITIVE_INTEGER,
            BaseType::Short => SHORT,
            BaseType::UnsignedLong => UNSIGNED_LONG,
            BaseType::UnsignedInt => UNSIGNED_INT,
            BaseType::UnsignedShort => UNSIGNED_SHORT,
            BaseType::UnsignedByte => UNSIGNED_BYTE,
            // Strings
            BaseType::ID => ID,
            BaseType::IDREF => IDREF,
            BaseType::Language => LANGUAGE,
            BaseType::Name => NAME,
            BaseType::NmToken => NMTOKEN,
            BaseType::NormalizedString => NORMALIZED_STRING,
            BaseType::String => STRING,
            BaseType::Token => TOKEN,
            // Dates
            BaseType::Date => DATE,
            BaseType::DateTime => DATETIME,
            BaseType::Duration => DURATION,
            BaseType::GDay => G_DAY,
            BaseType::GMonth => G_MONTH,
            BaseType::GMonthDay => G_MONTH_DAY,
            BaseType::GYear => G_YEAR,
            BaseType::GYearMonth => G_YEAR_MONTH,
            BaseType::Time => TIME,
            // Custom
            BaseType::Custom(o) => o.as_str(),
        }
    }
}

impl Display for BaseType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.name())
    }
}

impl BaseType {
    pub fn primitive(&self) -> Primitive {
        match self {
            // Numerics
            BaseType::Byte => Primitive::Numeric,
            BaseType::Decimal => Primitive::Numeric,
            BaseType::Int => Primitive::Numeric,
            BaseType::Integer => Primitive::Numeric,
            BaseType::Long => Primitive::Numeric,
            BaseType::NegativeInteger => Primitive::Numeric,
            BaseType::NonNegativeInteger => Primitive::Numeric,
            BaseType::NonPositiveInteger => Primitive::Numeric,
            BaseType::PositiveInteger => Primitive::Numeric,
            BaseType::Short => Primitive::Numeric,
            BaseType::UnsignedLong => Primitive::Numeric,
            BaseType::UnsignedInt => Primitive::Numeric,
            BaseType::UnsignedShort => Primitive::Numeric,
            BaseType::UnsignedByte => Primitive::Numeric,
            // Strings
            BaseType::ID => Primitive::Character,
            BaseType::IDREF => Primitive::Character,
            BaseType::Language => Primitive::Character,
            BaseType::Name => Primitive::Character,
            BaseType::NmToken => Primitive::Character,
            BaseType::NormalizedString => Primitive::Character,
            BaseType::String => Primitive::Character,
            BaseType::Token => Primitive::Character,
            // Dates
            BaseType::Date => Primitive::DateType,
            BaseType::DateTime => Primitive::DateType,
            BaseType::Duration => Primitive::DateType,
            BaseType::GDay => Primitive::DateType,
            BaseType::GMonth => Primitive::DateType,
            BaseType::GMonthDay => Primitive::DateType,
            BaseType::GYear => Primitive::DateType,
            BaseType::GYearMonth => Primitive::DateType,
            BaseType::Time => Primitive::DateType,
            // Custom
            BaseType::Custom(o) => Primitive::None,
        }
    }

    pub fn is_numeric(&self) -> bool {
        self.primitive() == Primitive::Numeric
    }

    pub fn is_character(&self) -> bool {
        self.primitive() == Primitive::Character
    }

    pub fn is_datetype(&self) -> bool {
        self.primitive() == Primitive::DateType
    }

    pub fn is_custom(&self) -> bool {
        self.primitive() == Primitive::None
    }

    pub fn as_numeric(&self) -> Option<Numeric> {
        match self {
            BaseType::Byte => Some(Numeric::Byte),
            BaseType::Decimal => Some(Numeric::Decimal),
            BaseType::Int => Some(Numeric::Int),
            BaseType::Integer => Some(Numeric::Integer),
            BaseType::Long => Some(Numeric::Long),
            BaseType::NegativeInteger => Some(Numeric::NegativeInteger),
            BaseType::NonNegativeInteger => Some(Numeric::NonNegativeInteger),
            BaseType::NonPositiveInteger => Some(Numeric::NonPositiveInteger),
            BaseType::PositiveInteger => Some(Numeric::PositiveInteger),
            BaseType::Short => Some(Numeric::Short),
            BaseType::UnsignedLong => Some(Numeric::UnsignedLong),
            BaseType::UnsignedInt => Some(Numeric::UnsignedInt),
            BaseType::UnsignedShort => Some(Numeric::UnsignedShort),
            BaseType::UnsignedByte => Some(Numeric::UnsignedByte),
            _ => None,
        }
    }

    pub fn as_character(&self) -> Option<Character> {
        match self {
            BaseType::ID => Some(Character::ID),
            BaseType::IDREF => Some(Character::IDREF),
            BaseType::Language => Some(Character::Language),
            BaseType::Name => Some(Character::Name),
            BaseType::NmToken => Some(Character::NmToken),
            BaseType::NormalizedString => Some(Character::NormalizedString),
            BaseType::String => Some(Character::String),
            BaseType::Token => Some(Character::Token),
            _ => None,
        }
    }

    pub fn as_datetime(&self) -> Option<DateTime> {
        match self {
            BaseType::Date => Some(DateTime::Date),
            BaseType::DateTime => Some(DateTime::DateTime),
            BaseType::Duration => Some(DateTime::Duration),
            BaseType::GDay => Some(DateTime::GDay),
            BaseType::GMonth => Some(DateTime::GMonth),
            BaseType::GMonthDay => Some(DateTime::GMonthDay),
            BaseType::GYear => Some(DateTime::GYear),
            BaseType::GYearMonth => Some(DateTime::GYearMonth),
            BaseType::Time => Some(DateTime::Time),
            _ => None,
        }
    }

    // pub fn parse_prefixed_or_custom<S1, S2>(s: S1, prefix: S2) -> Self
    // where
    //     S1: AsRef<str>,
    //     S2: AsRef<str>,
    // {
    //     match Self::parse_prefixed(&s, &prefix) {
    //         Ok(ok) => ok,
    //         Err(_) => {
    //             if prefix.as_ref().is_empty() {
    //
    //             } else {
    //                 BaseType::Custom(format!("{}:{}", prefix.as_ref(), s.as_ref()))
    //             }
    //         }
    //     }
    // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitive

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Primitive {
    None,
    Numeric,
    Character,
    DateType,
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Numeric Types

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

impl Default for Numeric {
    fn default() -> Self {
        Numeric::Int
    }
}

impl PrefixedParse for Numeric {
    type ParsedThing = Numeric;

    fn parse<S: AsRef<str>>(parseable: S) -> Result<Self::ParsedThing> {
        match parseable.as_ref() {
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
            _ => raise!("unknown numeric type: '{}'", parseable.as_ref()),
        }
    }
}

impl PrefixedString for Numeric {
    fn name(&self) -> &str {
        let s = match self {
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
        };
        s
    }
}

impl Display for Numeric {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.name())
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// String Types - named Character for disambiguation

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Character {
    ID,
    IDREF,
    Language,
    Name,
    NmToken,
    NormalizedString,
    String,
    Token,
}

impl Default for Character {
    fn default() -> Self {
        Character::String
    }
}

impl PrefixedParse for Character {
    type ParsedThing = Character;

    fn parse<S: AsRef<str>>(parseable: S) -> Result<Self::ParsedThing> {
        match parseable.as_ref() {
            ID => Ok(Character::ID),
            IDREF => Ok(Character::IDREF),
            LANGUAGE => Ok(Character::Language),
            NAME => Ok(Character::Name),
            NMTOKEN => Ok(Character::NmToken),
            NORMALIZED_STRING => Ok(Character::NormalizedString),
            STRING => Ok(Character::String),
            TOKEN => Ok(Character::Token),
            _ => raise!("unknown string type: '{}'", parseable.as_ref()),
        }
    }
}

impl PrefixedString for Character {
    fn name(&self) -> &str {
        let s = match self {
            Character::ID => ID,
            Character::IDREF => IDREF,
            Character::Language => LANGUAGE,
            Character::Name => NAME,
            Character::NmToken => NMTOKEN,
            Character::NormalizedString => NORMALIZED_STRING,
            Character::String => STRING,
            Character::Token => TOKEN,
        };
        s
    }
}

impl Display for Character {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.name())
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DateTime Types

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum DateTime {
    Date,
    DateTime,
    Duration,
    GDay,
    GMonth,
    GMonthDay,
    GYear,
    GYearMonth,
    Time,
}

impl PrefixedParse for DateTime {
    type ParsedThing = DateTime;

    fn parse<S: AsRef<str>>(parseable: S) -> Result<Self::ParsedThing> {
        match parseable.as_ref() {
            DATE => Ok(DateTime::Date),
            DATETIME => Ok(DateTime::DateTime),
            DURATION => Ok(DateTime::Duration),
            G_DAY => Ok(DateTime::GDay),
            G_MONTH => Ok(DateTime::GMonth),
            G_MONTH_DAY => Ok(DateTime::GMonthDay),
            G_YEAR => Ok(DateTime::GYear),
            G_YEAR_MONTH => Ok(DateTime::GYearMonth),
            TIME => Ok(DateTime::Time),
            _ => raise!("unknown datetime type: '{}'", parseable.as_ref()),
        }
    }
}

impl PrefixedString for DateTime {
    fn name(&self) -> &str {
        let s = match self {
            DateTime::Date => DATE,
            DateTime::DateTime => DATETIME,
            DateTime::Duration => DURATION,
            DateTime::GDay => G_DAY,
            DateTime::GMonth => G_MONTH,
            DateTime::GMonthDay => G_MONTH_DAY,
            DateTime::GYear => G_YEAR,
            DateTime::GYearMonth => G_YEAR_MONTH,
            DateTime::Time => TIME,
        };
        s
    }
}

impl Display for DateTime {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.name())
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Conveniences

impl BaseType {
    pub fn is_id(&self) -> bool {
        *self == BaseType::ID
    }
    pub fn is_idref(&self) -> bool {
        *self == BaseType::IDREF
    }
    pub fn is_language(&self) -> bool {
        *self == BaseType::Language
    }
    pub fn is_name(&self) -> bool {
        *self == BaseType::Name
    }
    pub fn is_nmtoken(&self) -> bool {
        *self == BaseType::NmToken
    }
    pub fn is_normalized_string(&self) -> bool {
        *self == BaseType::NormalizedString
    }
    pub fn is_string(&self) -> bool {
        *self == BaseType::String
    }
    pub fn is_token(&self) -> bool {
        *self == BaseType::Token
    }
    pub fn is_byte(&self) -> bool {
        *self == BaseType::Byte
    }
    pub fn is_decimal(&self) -> bool {
        *self == BaseType::Decimal
    }
    pub fn is_int(&self) -> bool {
        *self == BaseType::Int
    }
    pub fn is_integer(&self) -> bool {
        *self == BaseType::Integer
    }
    pub fn is_long(&self) -> bool {
        *self == BaseType::Long
    }
    pub fn is_negative_integer(&self) -> bool {
        *self == BaseType::NegativeInteger
    }
    pub fn is_non_negative_integer(&self) -> bool {
        *self == BaseType::NonNegativeInteger
    }
    pub fn is_non_positive_integer(&self) -> bool {
        *self == BaseType::PositiveInteger
    }
    pub fn is_positive_integer(&self) -> bool {
        *self == BaseType::NonPositiveInteger
    }
    pub fn is_short(&self) -> bool {
        *self == BaseType::Short
    }
    pub fn is_unsigned_long(&self) -> bool {
        *self == BaseType::UnsignedLong
    }
    pub fn is_unsigned_int(&self) -> bool {
        *self == BaseType::UnsignedInt
    }
    pub fn is_unsigned_short(&self) -> bool {
        *self == BaseType::UnsignedShort
    }
    pub fn is_unsigned_byte(&self) -> bool {
        *self == BaseType::UnsignedByte
    }
    pub fn is_date(&self) -> bool {
        *self == BaseType::Date
    }
    pub fn is_datetime(&self) -> bool {
        *self == BaseType::DateTime
    }
    pub fn is_duration(&self) -> bool {
        *self == BaseType::Duration
    }
    pub fn is_gday(&self) -> bool {
        *self == BaseType::GDay
    }
    pub fn is_gmonth(&self) -> bool {
        *self == BaseType::GMonth
    }
    pub fn is_gmonthday(&self) -> bool {
        *self == BaseType::GMonthDay
    }
    pub fn is_gyear(&self) -> bool {
        *self == BaseType::GYear
    }
    pub fn is_gyearmonth(&self) -> bool {
        *self == BaseType::GYearMonth
    }
    pub fn is_time(&self) -> bool {
        *self == BaseType::Time
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Helpers

fn split_raw_str(raw_str: &str) -> (&str, &str) {
    let mut split = raw_str.split(":");
    let mut ns: &str = split.next().unwrap_or("");
    let mut val: &str = split.next().unwrap_or("");
    if val.is_empty() && !ns.is_empty() {
        // if there no val but there is an ns, then it means that ns was actually the val.
        // e.g. "blah" -> ns="blah" val="", so we swap to ns="" val="blah"
        val = ns;
        ns = "";
    }
    (ns, val)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests

#[test]
fn split_no_ns() {
    let input = "bloop";
    let got = split_raw_str(input);
    let want = ("", "bloop");
    assert_eq!(got, want);
}

#[test]
fn split_with_ns() {
    let input = "x:bloop";
    let got = split_raw_str(input);
    let want = ("x", "bloop");
    assert_eq!(got, want);
}

#[test]
fn numeric_parse_prefixed() {
    let input = "xs:decimal";
    let got = Numeric::parse_prefixed(input, "xs").unwrap();
    let want = Numeric::Decimal;
    assert_eq!(got, want);
}

#[test]
fn numeric_parse_bad_prefix() {
    let input = "foo:decimal";
    let result = Numeric::parse_prefixed(input, "bar");
    assert!(result.is_err());
}

#[test]
fn numeric_parse_bad_value() {
    let input = "xs:decimate";
    let result = Numeric::parse_prefixed(input, "xs");
    assert!(result.is_err());
}

#[test]
fn parse_primitive_xs_datetime() {
    let input = "xs:dateTime";
    let got = BaseType::parse_prefixed(input, "xs").unwrap();
    let want = BaseType::DateTime;
    assert_eq!(got, want);
}

#[test]
fn parse_primitive_xs_string() {
    let input = "xs:string";
    let got = BaseType::parse_prefixed(input, "xs").unwrap();
    let want = BaseType::String;
    assert_eq!(got, want);
}

#[test]
fn parse_primitive_xs_byte() {
    let input = "floop:byte";
    let got = BaseType::parse_prefixed(input, "floop").unwrap();
    let want = BaseType::Byte;
    assert_eq!(got, want);
}

#[test]
fn parse_base_type() {
    struct TestCase {
        prefix: &'static str,
        input: &'static str,
        want: BaseType,
    }
    let test_cases = vec![TestCase {
        prefix: "xs",
        input: "xs:string",
        want: BaseType::String,
    }];
    for test_case in &test_cases {
        let got = BaseType::parse_prefixed(test_case.input, test_case.prefix).unwrap();
        assert_eq!(&got, &test_case.want);
    }
}

#[test]
fn parse_base_type_custom() {
    struct TestCase {
        prefix: &'static str,
        input: &'static str,
        want: BaseType,
    }
    let test_cases = vec![
        TestCase {
            prefix: "foo",
            input: "foo:bar",
            want: BaseType::Custom("foo:bar".to_owned()),
        },
        TestCase {
            prefix: "xs",
            input: "bloop:blerp",
            want: BaseType::Custom("bloop:blerp".to_owned()),
        },
    ];
    for test_case in &test_cases {
        let got = BaseType::parse_prefixed(test_case.input, test_case.prefix).unwrap();
        assert_eq!(&got, &test_case.want);
    }
}

#[test]
fn display_numeric() {
    let numeric = Numeric::Byte;
    let got = format!("{}", numeric);
    let want = "byte";
    assert_eq!(got.as_str(), want);
}

#[test]
fn display_base_type() {
    let base_type = BaseType::Byte;
    let got = format!("{}", base_type);
    let want = "byte";
    assert_eq!(got.as_str(), want);
}
