// TODO - constants are bad
pub const LINE_WIDTH: usize = 100;
pub const INDENT: &str = "    ";
pub const DOC_COMMENT: &str = "///";

use std::cmp::Ordering;
use std::fmt::{Display, Formatter};
use std::io::Write;

#[derive(Debug, Clone, Eq, Ord, Default)]
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

#[derive(Debug, Clone, Eq, Ord)]
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

pub fn tokenize<S: AsRef<str>>(s: S) -> Vec<String> {
    let mut tokens = Vec::new();
    let mut buf = String::new();
    let mut is_word_start = true;
    let mut was_lowercase = true;
    let mut was_digit = false;
    for c in s.as_ref().chars() {
        if was_lowercase && c.is_uppercase() {
            is_word_start = true;
        }
        if c.is_ascii_digit() {
            is_word_start = !was_digit;
        }
        if !c.is_alphanumeric() {
            is_word_start = true;
        } else if is_word_start {
            if !buf.is_empty() {
                tokens.push(buf);
                buf = String::new();
            }
            buf.push(c.to_ascii_lowercase());
            is_word_start = false;
        } else {
            buf.push(c.to_ascii_lowercase());
            is_word_start = false;
        }
        was_lowercase = !c.is_ascii_digit() && c.is_lowercase();
        was_digit = c.is_ascii_digit();
    }
    if !buf.is_empty() {
        tokens.push(buf);
    }
    tokens
}

pub fn pascal_case<S: AsRef<str>>(s: S) -> Symbol {
    case(s, Case::Pascal)
}

pub fn camel_case<S: AsRef<str>>(s: S) -> Symbol {
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
            out.push(c);
            is_next_upper = false;
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

pub fn sep<S: AsRef<str>>(s: S, subtract_indentations: usize) -> String {
    let mut out = DOC_COMMENT.to_owned();
    let width = LINE_WIDTH - (subtract_indentations * 4);
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

pub fn write_documentation<W, S>(w: &mut W, documentation: S, indents: usize) -> std::io::Result<()>
where
    W: Write,
    S: AsRef<str>,
{
    let words = words(documentation);
    let mut pos: usize = linestart(w, indents, false)?;
    let line_width = LINE_WIDTH - (indents * INDENT.len());
    for word in words {
        if word.as_str() == "\n" {
            writeln!(w)?;
            linestart(w, indents, false)?;
            writeln!(w)?;
            pos = linestart(w, indents, false)?;
        } else if !word.is_empty() && word.len() + pos + 1 >= line_width {
            writeln!(w)?;
            pos = linestart(w, indents, false)?;
        }
        if !word.is_empty() && word.as_str() != "\n" {
            write!(w, " {}", word.as_str())?;
            pos = pos + word.len() + 1;
        }
    }
    Ok(())
}

pub fn linestart<W: Write>(w: &mut W, indents: usize, add_space: bool) -> std::io::Result<usize> {
    let mut pos: usize = 0;
    for _ in 0..indents {
        write!(w, "{}", INDENT)?;
        pos += INDENT.len();
    }
    write!(w, "{}", DOC_COMMENT)?;
    pos += DOC_COMMENT.len();
    if add_space {
        write!(w, " ")?;
        pos += 1;
    }
    Ok(pos)
}

pub fn words<S: AsRef<str>>(s: S) -> Vec<String> {
    let mut result = Vec::new();
    let mut consecutive_newlines = 0;
    let mut word = String::new();
    for c in s.as_ref().chars() {
        if c.is_whitespace() {
            if !word.is_empty() {
                result.push(word.clone());
                word.clear();
            }
            if c == '\n' {
                if consecutive_newlines == 0 {
                    result.push("\n".into());
                }
                consecutive_newlines += 1;
                continue;
            }
        } else {
            word.push(c);
        }
        consecutive_newlines = 0;
    }
    if !word.is_empty() {
        result.push(word);
    }
    result
}

#[cfg(test)]
mod tests {
    use super::{words, Symbol};
    use crate::utils::string_stuff::tokenize;
    use std::io::Cursor;

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
        let input = "dominant-11th";
        let got = super::pascal_case(input);
        let want = "Dominant11th".to_string();
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
        let want = "dominant11th".to_string();
        assert_eq!(got.value(), want);
        if let Symbol::Altered(altered) = got {
            assert_eq!(altered.original.as_str(), input);
        } else {
            panic!("expected Symbol::Altered");
        }
    }

    #[test]
    fn get_words() {
        let input = r#"Here is a sentence. And another sentence, which has a comma:
        With extra spaces and newlines, etc."#;
        let got = words(input);
        let want = vec![
            "Here".to_owned(),
            "is".to_owned(),
            "a".to_owned(),
            "sentence.".to_owned(),
            "And".to_owned(),
            "another".to_owned(),
            "sentence,".to_owned(),
            "which".to_owned(),
            "has".to_owned(),
            "a".to_owned(),
            "comma:".to_owned(),
            "\n".to_owned(),
            "With".to_owned(),
            "extra".to_owned(),
            "spaces".to_owned(),
            "and".to_owned(),
            "newlines,".to_owned(),
            "etc.".to_owned(),
        ];
        assert_eq!(got.len(), want.len());
        for (i, want_word) in want.iter().enumerate() {
            let got_word = got.get(i).unwrap();
            assert_eq!(got_word, want_word);
        }
    }

    #[test]
    fn write_documentation() {
        let input = r#"Here is a sentence. And another sentence, which has a comma:
        With extra spaces and newlines, etc."#;
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// Here is a sentence. And another sentence, which has a comma:
    ///
    /// With extra spaces and newlines, etc."#;
        assert_eq!(got, want);
    }

    #[test]
    fn write_documentation_wrap() {
        let input = "Here is a sentence. And another sentence, which has a comma: without a \
        newline it goes on for a long time and must be wrapped it's really quite remarkable";
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// Here is a sentence. And another sentence, which has a comma: without a newline it goes
    /// on for a long time and must be wrapped it's really quite remarkable"#;
        assert_eq!(got, want);
    }

    #[test]
    fn write_documentation_newlines() {
        let input = "a\nb\n\nc\n\n\n\nd";
        let mut w = Cursor::new(vec![0; 15]);
        super::write_documentation(&mut w, input, 1).unwrap();
        let got = String::from_utf8_lossy(w.get_ref());
        let want = r#"    /// a
    ///
    /// b
    ///
    /// c
    ///
    /// d"#;
        assert_eq!(got, want);
    }

    #[test]
    fn tokenize_strings() {
        struct TestCase {
            input: &'static str,
            want: Vec<String>,
        }
        let mut test_cases = vec![
            TestCase {
                input: "modeValue",
                want: vec!["mode".to_owned(), "value".to_owned()],
            },
            TestCase {
                input: "DomInAnt11th",
                want: vec![
                    "dom".to_owned(),
                    "in".to_owned(),
                    "ant".to_owned(),
                    "11th".to_owned(),
                ],
            },
            TestCase {
                input: "dominant11th",
                want: vec!["dominant".to_owned(), "11th".to_owned()],
            },
        ];
        for test_case in &test_cases {
            let got = tokenize(test_case.input);
            assert_eq!(got, test_case.want);
        }
    }
}
