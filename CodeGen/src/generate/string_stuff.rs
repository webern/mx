use std::cmp::Ordering;
use std::fmt::{Display, Formatter};

#[derive(Debug, Clone, Eq, Ord, Hash, Default)]
pub struct Altered {
    pub value: String,
    pub original: String,
}

impl Display for Altered {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}({})", self.value, self.original)
    }
}

impl PartialOrd for Altered {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.value.cmp(&other.value))
    }
}

impl PartialEq for Altered {
    fn eq(&self, other: &Altered) -> bool {
        self.value == other.value
    }
}

#[derive(Debug, Clone, Eq, Ord, Hash)]
pub enum Symbol {
    Unaltered(String),
    Altered(Altered),
}

impl PartialOrd for Symbol {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.value().cmp(other.value()))
    }
}

impl PartialEq for Symbol {
    fn eq(&self, other: &Symbol) -> bool {
        self.value() == other.value()
    }
}

impl Symbol {
    pub fn new<S: AsRef<str>>(s: S) -> Symbol {
        Symbol::Unaltered(s.as_ref().to_string())
    }

    pub fn value(&self) -> &str {
        match self {
            Symbol::Unaltered(s) => s.as_str(),
            Symbol::Altered(a) => a.value.as_str(),
        }
    }
}

pub(crate) fn pascal_case<S: AsRef<str>>(s: S) -> Symbol {
    let mut out = String::new();
    let mut upper = true;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            out.push(c);
        }
        upper = false;
    }
    if out.as_str() == s.as_ref() {
        Symbol::Unaltered(out)
    } else {
        Symbol::Altered(Altered {
            value: out,
            original: s.as_ref().to_owned(),
        })
    }
}

pub(crate) fn camel_case<S: AsRef<str>>(s: S) -> Symbol {
    let mut out = String::new();
    let mut upper = false;
    for c in s.as_ref().chars() {
        if c == '-' || c == ' ' {
            upper = true;
            continue;
        }
        if upper {
            let uc = c.to_uppercase().next().unwrap();
            out.push(uc);
        } else {
            let uc = c.to_lowercase().next().unwrap();
            out.push(c);
        }
        upper = false;
    }

    // StepEnum is coming out wrong, don't know why.
    if out.len() == 1 {
        out = out.to_lowercase();
    }

    if out.as_str() == s.as_ref() {
        Symbol::Unaltered(out)
    } else {
        Symbol::Altered(Altered {
            value: out,
            original: s.as_ref().to_owned(),
        })
    }
}
