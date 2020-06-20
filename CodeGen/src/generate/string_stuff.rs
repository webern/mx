use std::cmp::Ordering;
use std::fmt::{Display, Formatter};
use crate::generate::mx_writer::HAPPY_WIDTH;

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

    /// If `altered` and `original` are equal, returns `Unaltered`, else returns `Altered`.
    pub fn from<S1, S2>(altered: S1, original: S2) -> Self
        where
            S1: AsRef<str>,
            S2: AsRef<str>,
    {
        if altered.as_ref() == original.as_ref() {
            Symbol::new(original.as_ref())
        } else {
            Symbol::Altered(Altered {
                value: altered.as_ref().into(),
                original: original.as_ref().into(),
            })
        }
    }

    pub fn value(&self) -> &str {
        match self {
            Symbol::Unaltered(s) => s.as_str(),
            Symbol::Altered(a) => a.value.as_str(),
        }
    }

    pub fn original(&self) -> &str {
        match self {
            Symbol::Unaltered(s) => s.as_str(),
            Symbol::Altered(a) => a.original.as_str(),
        }
    }
}

pub(crate) fn pascal_case<S: AsRef<str>>(s: S) -> Symbol {
    case(s, Case::Pascal)
}

pub(crate) fn camel_case<S: AsRef<str>>(s: S) -> Symbol {
    case(s, Case::Camel)
}

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
enum Case {
    Camel,
    Pascal,
}

fn case<S: AsRef<str>>(s: S, cs: Case) -> Symbol {
    let mut out = String::new();
    let mut is_next_upper = cs == Case::Pascal;
    for c in s.as_ref().chars() {
        if !c.is_alphanumeric() {
            is_next_upper = true;
        } else if c.is_ascii_digit() {
            // idiosyncrasy of the first implementation
            out.push(c);
            is_next_upper = true;
        } else if is_next_upper {
            out.push(c.to_ascii_uppercase());
            is_next_upper = false;
        } else {
            out.push(c.to_ascii_lowercase());
            is_next_upper = false;
        }
    }
    Symbol::from(out, s)
}

#[cfg(test)]
mod tests {
    use crate::generate::string_stuff::Symbol;

    #[test]
    fn pascal_case_clef_tab() {
        let input = "TAB";
        let got = super::pascal_case(input);
        let want = "Tab".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_clef_tab() {
        let input = "TAB";
        let got = super::camel_case(input);
        let want = "tab".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn pascal_case_step_a_lower() {
        let input = "a";
        let got = super::pascal_case(input);
        let want = "A".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_a_lower() {
        let input = "a";
        let got = super::camel_case(input);
        let want = "a".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Unaltered(unaltered) = got {
            assert_eq!(unaltered.as_str(), input);
        } else {
            panic!("expected Symbol::Unaltered");
        }
    }

    #[test]
    fn pascal_case_wind_chimes() {
        let input = "wind cHIMES";
        let got = super::pascal_case(input);
        let want = "WindChimes".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_wind_chimes() {
        let input = "wind cHIMES";
        let got = super::camel_case(input);
        let want = "windChimes".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn pascal_case_chord_kind() {
        let input = "DomInAnt11th";
        let got = super::pascal_case(input);
        let want = "Dominant11Th".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn camel_case_step_chord_kind() {
        let input = "DomInAnt 1 1th";
        let got = super::camel_case(input);
        let want = "dominant11Th".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }
}

pub fn sep<S: AsRef<str>>(s: S, subtract_indentations: usize) -> String {
    let mut out = "//".to_owned();
    let width = (HAPPY_WIDTH - (subtract_indentations * 4));
    let s = s.as_ref();
    if !s.is_empty() {
        out.push_str(format!(" {} ", s).as_str());
    }
    loop {
        if out.len() >= width {
            break;
        }
        out.push('/');
    }
    out
}