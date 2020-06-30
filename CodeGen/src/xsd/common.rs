use std::fmt::{Display, Formatter};

#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Debug)]
pub enum DefinedBy {
    Ref(String),
    Type(String),
}

impl DefinedBy {
    pub fn value(&self) -> &str {
        match self {
            DefinedBy::Ref(s) => s.as_str(),
            DefinedBy::Type(s) => s.as_str(),
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
