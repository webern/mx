use std::fmt::{Display, Formatter};

#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Debug)]
pub enum DefinedBy {
    Ref(String),
    Type(String),
}

impl DefinedBy {
    /// A quicker way, if you don't care whether it is a Ref or a Def
    pub fn value(&self) -> &str {
        match self {
            DefinedBy::Ref(s) => s.as_str(),
            DefinedBy::Type(s) => s.as_str(),
        }
    }

    pub fn is_ref(&self) -> bool {
        match self {
            DefinedBy::Ref(_) => true,
            _ => false,
        }
    }

    pub fn is_type(&self) -> bool {
        match self {
            DefinedBy::Type(_) => true,
            _ => false,
        }
    }
}

impl Display for DefinedBy {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            DefinedBy::Ref(s) => write!(f, "ref:{}", s),
            DefinedBy::Type(s) => write!(f, "type:{}", s),
        }
    }
}

#[test]
fn defined_by_tests() {
    let t = DefinedBy::Type("x".into());
    assert!(t.is_type());
    assert!(!t.is_ref());
    assert_eq!(t.value(), "x");
    let r = DefinedBy::Ref("y".into());
    assert!(!r.is_type());
    assert!(r.is_ref());
    assert_eq!(r.value(), "y");
    let ts = format!("{}", &t);
    assert_eq!("type:x", ts.as_str());
    let rs = format!("{}", &r);
    assert_eq!("ref:y", rs.as_str());
}
