#[macro_use]
use crate::model::create::{CreateError, CreateResult};
use crate::model::scalar::{Bound, Range, ScalarNumeric, ScalarNumericData, ScalarString};
use crate::model::symbol::Symbol;
use crate::model::Model;
use crate::xsd::primitives::{BaseType, Character, Numeric, Primitive};
use crate::xsd::restriction::{Facet, Number, Restriction};
use crate::xsd::simple_type::{Payload, SimpleType};
use crate::xsd::Xsd;

pub(super) fn model_scalar_string(st: &SimpleType, _xsd: &Xsd) -> Option<CreateResult> {
    match &st.payload {
        Payload::Restriction(r) => {
            if r.facets.is_empty() {
                return None;
            }
            let base_type = if let BaseType::Primitive(p) = r.base {
                p
            } else {
                // not a primitive type (such as string)
                return None;
            };
            let base_type = if let Primitive::Character(c) = base_type {
                c
            } else {
                // not a character (i.e. string) type
                return None;
            };
            let mut scalar_string = match base_type {
                Character::Language | Character::Name | Character::NormalizedString => {
                    return some_create_err!("'{}' is not supported", base_type);
                }
                _ => ScalarString {
                    name: Symbol::new(st.name.as_str()),
                    base_type,
                    documentation: st.documentation(),
                    ..Default::default()
                },
            };
            for facet in &r.facets {
                match facet {
                    Facet::Enumeration(_)
                    | Facet::MaxExclusive(_)
                    | Facet::MaxInclusive(_)
                    | Facet::MinExclusive(_)
                    | Facet::MinInclusive(_) => {
                        return some_create_err!("unsupported facet '{:?}'", facet)
                    }
                    Facet::Length(l) => {
                        scalar_string.min_length = Some(*l);
                        scalar_string.max_length = Some(*l);
                    }
                    Facet::MaxLength(l) => scalar_string.max_length = Some(*l),
                    Facet::MinLength(l) => scalar_string.min_length = Some(*l),
                    Facet::Pattern(p) => scalar_string.pattern = Some(p.clone()),
                }
            }
            return Some(Ok(Some(vec![Model::ScalarString(scalar_string)])));
        }
        _ => return None,
    }
}

pub(super) fn model_scalar_number(st: &SimpleType, _xsd: &Xsd) -> Option<CreateResult> {
    match &st.payload {
        Payload::Restriction(r) => {
            let base_type = if let BaseType::Primitive(p) = r.base {
                p
            } else {
                // not a primitive type (such as int or float)
                return None;
            };
            let base_type = if let Primitive::Numeric(n) = base_type {
                n
            } else {
                // not a numeric type
                return None;
            };
            return Some(produce_the_scalar_numeric(base_type, r, st, _xsd));
        }
        _ => return None,
    }
}

fn produce_the_scalar_numeric(
    base_type: Numeric,
    r: &Restriction,
    st: &SimpleType,
    _xsd: &Xsd,
) -> CreateResult {
    let mut scalar_numeric = match base_type {
        Numeric::Byte | Numeric::UnsignedByte => {
            return Err(make_create_err!(
                "Unsupported numeric type: '{}'",
                base_type
            ))
        }
        Numeric::Decimal => ScalarNumeric::Decimal(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_decimal_range(Range::default(), &r.facets)?,
        }),

        Numeric::NegativeInteger => ScalarNumeric::Integer(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_integer_range(Range::new(None, Some(Bound::Inclusive(-1))), &r.facets)?,
        }),
        Numeric::NonNegativeInteger => ScalarNumeric::Integer(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_integer_range(Range::new(Some(Bound::Inclusive(0)), None), &r.facets)?,
        }),
        Numeric::NonPositiveInteger => ScalarNumeric::Integer(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_integer_range(Range::new(None, Some(Bound::Inclusive(0))), &r.facets)?,
        }),
        Numeric::PositiveInteger => ScalarNumeric::Integer(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_integer_range(Range::new(Some(Bound::Inclusive(1)), None), &r.facets)?,
        }),

        Numeric::Int
        | Numeric::Integer
        | Numeric::Long
        | Numeric::Short
        | Numeric::UnsignedLong
        | Numeric::UnsignedInt
        | Numeric::UnsignedShort => ScalarNumeric::Integer(ScalarNumericData {
            name: Symbol::new(st.name.as_str()),
            base_type,
            documentation: st.documentation(),
            range: parse_integer_range(Range::default(), &r.facets)?,
        }),
    };
    Ok(Some(vec![Model::ScalarNumber(scalar_numeric)]))
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Wow what an incredible mess

fn parse_decimal_range(
    starting_range: Range<f64>,
    facets: &[Facet],
) -> std::result::Result<Range<f64>, CreateError> {
    let mut min = starting_range.min.and_then(|some| match some {
        Bound::Inclusive(i) => Some(LowerFloatBound::Inclusive(i)),
        Bound::Exclusive(e) => Some(LowerFloatBound::Exclusive(e)),
    });
    let mut max = starting_range.max.and_then(|some| match some {
        Bound::Inclusive(i) => Some(UpperFloatBound::Inclusive(i)),
        Bound::Exclusive(e) => Some(UpperFloatBound::Exclusive(e)),
    });
    for facet in facets {
        match facet {
            Facet::Enumeration(_)
            | Facet::Length(_)
            | Facet::MaxLength(_)
            | Facet::MinLength(_)
            | Facet::Pattern(_) => return Err(make_create_err!("unsupported facet '{:?}'", facet)),

            Facet::MaxExclusive(n) => {
                let new_upper = UpperFloatBound::Exclusive(match n {
                    Number::Integer(i) => *i as f64,
                    Number::Decimal(f) => *f,
                });
                max = match max {
                    None => Some(new_upper),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_upper) {
                            Some(new_upper)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MaxInclusive(n) => {
                let new_upper = UpperFloatBound::Inclusive(match n {
                    Number::Integer(i) => *i as f64,
                    Number::Decimal(f) => *f,
                });
                max = match max {
                    None => Some(new_upper),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_upper) {
                            Some(new_upper)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MinExclusive(n) => {
                let new_lower = LowerFloatBound::Exclusive(match n {
                    Number::Integer(i) => *i as f64,
                    Number::Decimal(f) => *f,
                });
                min = match min {
                    None => Some(new_lower),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_lower) {
                            Some(new_lower)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MinInclusive(n) => {
                let new_lower = LowerFloatBound::Inclusive(match n {
                    Number::Integer(i) => *i as f64,
                    Number::Decimal(f) => *f,
                });
                min = match min {
                    None => Some(new_lower),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_lower) {
                            Some(new_lower)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
        }
    }
    Ok(Range {
        min: min.and_then(|some| match some {
            LowerFloatBound::Inclusive(x) => Some(Bound::Inclusive(x)),
            LowerFloatBound::Exclusive(x) => Some(Bound::Exclusive(x)),
        }),
        max: max.and_then(|some| match some {
            UpperFloatBound::Inclusive(x) => Some(Bound::Inclusive(x)),
            UpperFloatBound::Exclusive(x) => Some(Bound::Inclusive(x)),
        }),
    })
}

fn parse_integer_range(
    starting_range: Range<i64>,
    facets: &[Facet],
) -> std::result::Result<Range<i64>, CreateError> {
    let mut min = starting_range.min.and_then(|some| match some {
        Bound::Inclusive(i) => Some(LowerIntegerBound::Inclusive(i)),
        Bound::Exclusive(e) => Some(LowerIntegerBound::Exclusive(e)),
    });
    let mut max = starting_range.max.and_then(|some| match some {
        Bound::Inclusive(i) => Some(UpperIntegerBound::Inclusive(i)),
        Bound::Exclusive(e) => Some(UpperIntegerBound::Exclusive(e)),
    });
    for facet in facets {
        match facet {
            Facet::Enumeration(_)
            | Facet::Length(_)
            | Facet::MaxLength(_)
            | Facet::MinLength(_)
            | Facet::Pattern(_) => return Err(make_create_err!("unsupported facet '{:?}'", facet)),

            Facet::MaxExclusive(n) => {
                let new_upper = UpperIntegerBound::Exclusive(match n {
                    Number::Integer(i) => *i,
                    Number::Decimal(f) => *f as i64,
                });
                max = match max {
                    None => Some(new_upper),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_upper) {
                            Some(new_upper)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MaxInclusive(n) => {
                let new_upper = UpperIntegerBound::Inclusive(match n {
                    Number::Integer(i) => *i,
                    Number::Decimal(f) => *f as i64,
                });
                max = match max {
                    None => Some(new_upper),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_upper) {
                            Some(new_upper)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MinExclusive(n) => {
                let new_lower = LowerIntegerBound::Exclusive(match n {
                    Number::Integer(i) => *i,
                    Number::Decimal(f) => *f as i64,
                });
                min = match min {
                    None => Some(new_lower),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_lower) {
                            Some(new_lower)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
            Facet::MinInclusive(n) => {
                let new_lower = LowerIntegerBound::Inclusive(match n {
                    Number::Integer(i) => *i,
                    Number::Decimal(f) => *f as i64,
                });
                min = match min {
                    None => Some(new_lower),
                    Some(current) => {
                        if current.is_other_more_restrictive(&new_lower) {
                            Some(new_lower)
                        } else {
                            Some(current)
                        }
                    }
                }
            }
        }
    }
    Ok(Range {
        min: min.and_then(|some| match some {
            LowerIntegerBound::Inclusive(x) => Some(Bound::Inclusive(x)),
            LowerIntegerBound::Exclusive(x) => Some(Bound::Exclusive(x)),
        }),
        max: max.and_then(|some| match some {
            UpperIntegerBound::Inclusive(x) => Some(Bound::Inclusive(x)),
            UpperIntegerBound::Exclusive(x) => Some(Bound::Inclusive(x)),
        }),
    })
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Way too much internal stuff for figuring out ranges

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
pub enum UpperIntegerBound {
    Inclusive(i64),
    Exclusive(i64),
}

impl UpperIntegerBound {
    pub fn is_other_more_restrictive(&self, other: &UpperIntegerBound) -> bool {
        let my_max = match self {
            UpperIntegerBound::Inclusive(i) => *i,
            UpperIntegerBound::Exclusive(x) => x - 1,
        };
        let other_max = match other {
            UpperIntegerBound::Inclusive(i) => *i,
            UpperIntegerBound::Exclusive(x) => x - 1,
        };
        other_max < my_max
    }
}

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
pub enum LowerIntegerBound {
    Inclusive(i64),
    Exclusive(i64),
}

impl LowerIntegerBound {
    pub fn is_other_more_restrictive(&self, other: &LowerIntegerBound) -> bool {
        let my_min = match self {
            LowerIntegerBound::Inclusive(i) => *i,
            LowerIntegerBound::Exclusive(x) => x + 1,
        };
        let other_min = match other {
            LowerIntegerBound::Inclusive(i) => *i,
            LowerIntegerBound::Exclusive(x) => x + 1,
        };
        other_min > my_min
    }
}

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
pub enum UpperFloatBound {
    Inclusive(f64),
    Exclusive(f64),
}

impl UpperFloatBound {
    pub fn is_other_more_restrictive(&self, other: &UpperFloatBound) -> bool {
        let (my_val, my_is_exclusive) = match self {
            UpperFloatBound::Inclusive(i) => (*i, false),
            UpperFloatBound::Exclusive(x) => (*x, true),
        };
        let (ot_val, ot_is_exclusive) = match other {
            UpperFloatBound::Inclusive(i) => (*i, false),
            UpperFloatBound::Exclusive(x) => (*x, true),
        };
        if my_val == ot_val {
            if my_is_exclusive == ot_is_exclusive {
                // they are exactly the same
                return false;
            } else if ot_is_exclusive {
                // exclusive is more restrictive, so 'other' is more restrictive
                return true;
            } else {
                // exclusive is more restrictive, so 'self' is more restrictive
                return false;
            }
        }
        ot_val < my_val
    }
}

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
pub enum LowerFloatBound {
    Inclusive(f64),
    Exclusive(f64),
}

impl LowerFloatBound {
    pub fn is_other_more_restrictive(&self, other: &LowerFloatBound) -> bool {
        let (my_val, my_is_exclusive) = match self {
            LowerFloatBound::Inclusive(i) => (*i, false),
            LowerFloatBound::Exclusive(x) => (*x, true),
        };
        let (ot_val, ot_is_exclusive) = match other {
            LowerFloatBound::Inclusive(i) => (*i, false),
            LowerFloatBound::Exclusive(x) => (*x, true),
        };
        if my_val == ot_val {
            if my_is_exclusive == ot_is_exclusive {
                // they are exactly the same
                return false;
            } else if ot_is_exclusive {
                // exclusive is more restrictive, so 'other' is more restrictive
                return true;
            } else {
                // exclusive is more restrictive, so 'self' is more restrictive
                return false;
            }
        }
        ot_val > my_val
    }
}

#[test]
fn upper_integer_bound_is_other_more_restrictive() {
    #[derive(Clone, Copy, Debug)]
    struct TestCase {
        the_self: UpperIntegerBound,
        other: UpperIntegerBound,
        want: bool,
    }
    let test_cases = vec![
        TestCase {
            the_self: UpperIntegerBound::Inclusive(1),
            other: UpperIntegerBound::Inclusive(1),
            want: false,
        },
        TestCase {
            the_self: UpperIntegerBound::Inclusive(1),
            other: UpperIntegerBound::Exclusive(1),
            want: true,
        },
        TestCase {
            the_self: UpperIntegerBound::Exclusive(1),
            other: UpperIntegerBound::Inclusive(1),
            want: false,
        },
        TestCase {
            the_self: UpperIntegerBound::Exclusive(1),
            other: UpperIntegerBound::Exclusive(1),
            want: false,
        },
        TestCase {
            the_self: UpperIntegerBound::Inclusive(10),
            other: UpperIntegerBound::Exclusive(20),
            want: false,
        },
        TestCase {
            the_self: UpperIntegerBound::Inclusive(20),
            other: UpperIntegerBound::Exclusive(10),
            want: true,
        },
    ];
    for t in &test_cases {
        let got = t.the_self.is_other_more_restrictive(&t.other);
        assert_eq!(got, t.want, "'{:?}", t)
    }
}

#[test]
fn lower_integer_bound_is_other_more_restrictive() {
    #[derive(Clone, Copy, Debug)]
    struct TestCase {
        the_self: LowerIntegerBound,
        other: LowerIntegerBound,
        want: bool,
    }
    let test_cases = vec![
        TestCase {
            the_self: LowerIntegerBound::Inclusive(1),
            other: LowerIntegerBound::Inclusive(1),
            want: false,
        },
        TestCase {
            the_self: LowerIntegerBound::Inclusive(1),
            other: LowerIntegerBound::Exclusive(1),
            want: true,
        },
        TestCase {
            the_self: LowerIntegerBound::Exclusive(1),
            other: LowerIntegerBound::Inclusive(1),
            want: false,
        },
        TestCase {
            the_self: LowerIntegerBound::Exclusive(1),
            other: LowerIntegerBound::Exclusive(1),
            want: false,
        },
        TestCase {
            the_self: LowerIntegerBound::Inclusive(10),
            other: LowerIntegerBound::Exclusive(20),
            want: true,
        },
        TestCase {
            the_self: LowerIntegerBound::Inclusive(20),
            other: LowerIntegerBound::Exclusive(10),
            want: false,
        },
    ];
    for t in &test_cases {
        let got = t.the_self.is_other_more_restrictive(&t.other);
        assert_eq!(got, t.want, "'{:?}", t)
    }
}

#[test]
fn upper_float_bound_is_other_more_restrictive() {
    #[derive(Clone, Copy, Debug)]
    struct TestCase {
        the_self: UpperFloatBound,
        other: UpperFloatBound,
        want: bool,
    }
    let test_cases = vec![
        TestCase {
            the_self: UpperFloatBound::Inclusive(1.0),
            other: UpperFloatBound::Inclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: UpperFloatBound::Inclusive(1.0),
            other: UpperFloatBound::Exclusive(1.0),
            want: true,
        },
        TestCase {
            the_self: UpperFloatBound::Exclusive(1.0),
            other: UpperFloatBound::Inclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: UpperFloatBound::Exclusive(1.0),
            other: UpperFloatBound::Exclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: UpperFloatBound::Inclusive(0.01),
            other: UpperFloatBound::Exclusive(0.02),
            want: false,
        },
        TestCase {
            the_self: UpperFloatBound::Inclusive(0.02),
            other: UpperFloatBound::Exclusive(0.01),
            want: true,
        },
    ];
    for t in &test_cases {
        let got = t.the_self.is_other_more_restrictive(&t.other);
        assert_eq!(got, t.want, "'{:?}", t)
    }
}

#[test]
fn lower_float_bound_is_other_more_restrictive() {
    #[derive(Clone, Copy, Debug)]
    struct TestCase {
        the_self: LowerFloatBound,
        other: LowerFloatBound,
        want: bool,
    }
    let test_cases = vec![
        TestCase {
            the_self: LowerFloatBound::Inclusive(1.0),
            other: LowerFloatBound::Inclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: LowerFloatBound::Inclusive(1.0),
            other: LowerFloatBound::Exclusive(1.0),
            want: true,
        },
        TestCase {
            the_self: LowerFloatBound::Exclusive(1.0),
            other: LowerFloatBound::Inclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: LowerFloatBound::Exclusive(1.0),
            other: LowerFloatBound::Exclusive(1.0),
            want: false,
        },
        TestCase {
            the_self: LowerFloatBound::Inclusive(0.01),
            other: LowerFloatBound::Exclusive(0.02),
            want: true,
        },
        TestCase {
            the_self: LowerFloatBound::Inclusive(0.02),
            other: LowerFloatBound::Exclusive(0.01),
            want: false,
        },
    ];
    for t in &test_cases {
        let got = t.the_self.is_other_more_restrictive(&t.other);
        assert_eq!(got, t.want, "'{:?}", t)
    }
}
